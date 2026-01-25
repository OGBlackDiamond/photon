#version 460 core

out vec4 FragColor;
in vec2 pixelPosition;

uniform vec3 pixel00Pos;
uniform vec3 pixelDeltaU;
uniform vec3 pixelDeltaV;

uniform int numSpheres;
uniform int numTris;

uniform int frameCount;
uniform int screenWidth;
uniform int screenHeight;

uniform bool isAccumulating;
uniform sampler2D prevTex;
uniform sampler2D accumTex;

struct Ray {
    vec3 position;
    vec3 direction;
};

struct Surface {
    vec3 surfaceColor;
    vec3 emissionColor;
    float emissionStrength;
    float smoothness;
};

struct HitInfo {
    bool didHit;
    float distance;
    vec3 hitPoint;
    vec3 normal;
    Surface shapeSurface;
};

struct Sphere {
    vec3 center;
    float radius;
    Surface surface;
};

struct Triangle {
    vec3 center;
    vec3 v1, v2, v3;
    Surface surface;
};

layout(std430, binding=0) buffer Spheres {
    Sphere sphereData[];
};

layout(std430, binding=1) buffer Tris {
    Triangle triData[];
};



// linear interpolation
vec3 lerp(const vec3 v0, const vec3 v1, float weight) {
    return v0 + (v1 - v0) * weight;
}

HitInfo checkCollision(Ray ray, Triangle triangle) {
    normalize(ray.direction);

    triangle.v1 += triangle.center;
    triangle.v2 += triangle.center;
    triangle.v3 += triangle.center;

    vec3 E1 = triangle.v2 - triangle.v1;
    vec3 E2 = triangle.v3 - triangle.v1;

    vec3 N = cross(E1, E2);

    float det = -dot(ray.direction, N);
    float invdet = 1.0/det;

    vec3 AO = ray.position - triangle.v1;
    vec3 DAO = cross(AO, ray.direction);

    float u = dot(E2, DAO) * invdet;
    float v = -dot(E1, DAO) * invdet;
    float dist = dot(AO, N) * invdet;

    float w = 1 - u - v;

    HitInfo hit;

    hit.didHit = (det >= 1e-6 && dist >= 0.0 && u >= 0.0 && v >= 0.0 && (u+v) <= 1.0);

    if (hit.didHit) {
        hit.distance = dist;
        hit.hitPoint = ray.position + dist * ray.direction;
        hit.normal = normalize(N);
        hit.shapeSurface = triangle.surface;
    }

    return hit;
}

HitInfo checkCollision(Ray ray, Sphere sphere) {
    normalize(ray.direction);

    vec3 oc = sphere.center - ray.position;
    
    float a = pow(length(ray.direction), 2);
    float b = dot(oc, ray.direction);
    float c = pow(length(oc), 2) - pow(sphere.radius, 2);

    float dis = b * b - a * c;

    HitInfo hit;

    hit.didHit = false;

    if (dis >= 0) {
        float distance = (b -  sqrt(dis)) / a;
        if (distance > 0) {
            hit.didHit = true;
            hit.distance = distance;
            hit.hitPoint = ray.position + distance * ray.direction;
            hit.normal = normalize(hit.hitPoint - sphere.center);
            hit.shapeSurface = sphere.surface;
        }
    }

    return hit;
}

HitInfo calculateRayCollisions(Ray ray) {

    float closestToRay = 340282346638528859811704183484516925440.0000000000000000;

    HitInfo info;
    info.didHit = false;

    for (int i = 0; i < numSpheres; i++) {
        Sphere sphere = sphereData[i];

        HitInfo hit = checkCollision(ray, sphere);

        if (hit.didHit && hit.distance < closestToRay) {
            closestToRay = hit.distance;
            info = hit;
        }
    }

    for (int i = 0; i < numTris; i++) {
        Triangle tri = triData[i];

        HitInfo hit = checkCollision(ray, tri);
        if (hit.didHit && hit.distance < closestToRay) {
            closestToRay = hit.distance;
            info = hit;
        }
    }

    return info;
}

float randomValue(inout uint seed) {
    seed = seed * 747796405 + uint(2891336453) * frameCount;
    uint result = ((seed >> ((seed >> 28) + 4)) ^ seed) * 277803737;
    result = (result >> 22) ^ result;
    return result / 4294967295.0;
}

float randomNormalValue(inout uint seed) {
    float theta = 2 * 3.1415926 * randomValue(seed);
    float rho = sqrt(-2 * log(randomValue(seed)) / log(10)); 
    return rho * cos(theta);
}

vec3 randomDirection(inout uint seed) {
    float x = randomNormalValue(seed);
    float y = randomNormalValue(seed);
    float z = randomNormalValue(seed);
    vec3 direction = vec3(x, y, z);
    return normalize(direction);
}


vec3 traceRay(Ray ray, inout uint randomSeed) {
    vec3 color = vec3(1);
    vec3 light = vec3(0);

    for (int i = 0; i <= 10; i++) {
        HitInfo hit = calculateRayCollisions(ray);
        if (hit.didHit) {
            vec3 randomDir = normalize(hit.normal + randomDirection(randomSeed));
            vec3 specularDir = ray.direction - 2 * dot(ray.direction, hit.normal) * hit.normal;
            
            Surface surface = hit.shapeSurface;


            ray.position = hit.hitPoint;
            ray.direction = lerp(randomDir, specularDir, surface.smoothness);

            light += color * (surface.emissionColor * surface.emissionStrength);

            color *= surface.surfaceColor;
        } else break;
    }

    return light;
}


vec3 castRays() {
    uint randomSeed = uint((pixelPosition.y + 1) * 2500) * 500 + uint((pixelPosition.x + 1) * 2500) + (frameCount * 2056);

    float i = ((pixelPosition.x + 1.0) / 2.0) * screenWidth;
    float j = ((pixelPosition.y + 1.0) / 2.0) * screenHeight;
    vec3 pixelCenter = pixel00Pos + i * pixelDeltaU + j * pixelDeltaV;

    Ray ray = Ray(vec3(0, 0, 0), normalize(pixelCenter - vec3(0, 0, 0)));

    vec3 totalColor = vec3(0);

    for (int i = 0; i < 1; i++) {
        totalColor += traceRay(ray, randomSeed);
    }

    return totalColor;

}


void main() {
    vec2 texCoord = (pixelPosition + 1.0) * 0.5;
    if (isAccumulating) {
        vec3 currentColor = castRays();
        vec3 prevColor = vec3(0);
        if (frameCount > 1) {
            prevColor = texture(prevTex, texCoord).rgb;
        }
        vec3 accumulated = (prevColor * (frameCount - 1) + currentColor) / frameCount;
        FragColor = vec4(accumulated, 1);
    } else {
        FragColor = texture(accumTex, texCoord) * 10;
    }
}
