#version 460 core

out vec4 FragColor;
in vec2 pixelPosition;

uniform vec3 pixel00Pos;
uniform vec3 pixelDeltaU;
uniform vec3 pixelDeltaV;

uniform int numSpheres;
uniform int numTris;

// TODO: maybe make this not have to be set to a random number
// use buffers??

// position, color, emissionColor
uniform mat3 sphereVectors[100];
// radius, brightness, smoothness
uniform vec3 sphereFloats[100];

// v1, v2, v3
uniform mat3 trianglePoints[385];
// position, color, emissionColor
uniform mat3 triangleVectors[385];
// brightness, smoothness
uniform vec2 triangleFloats[385];

uniform int iterationCount;


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


Sphere sphere1 = Sphere(vec3(-0.75, 0, 3), 0.5, Surface(
    vec3(1, 0.2, 0.2),
    vec3(1, 1, 1),
    0,
    0
));

Sphere sphere2 = Sphere(vec3(1.75, 0, 2), 1.5, Surface(
    vec3(0, 0, 0),
    vec3(1, 1, 1),
    2,
    0
));

Triangle tri = Triangle(
    vec3(0, 0, 0),
    sphereVectors[0][0],
    sphereVectors[2][0],
    sphereVectors[1][0],
    Surface(
        vec3(1, 119.0/255.0, 188.0/255.0),
        vec3(0, 0, 0),
        0,
        0
    )
);


// linear interpolation
vec3 lerp(const vec3 v0, const vec3 v1, float weight) {
    return v0 + (v1 - v0) * weight;
}

HitInfo checkCollision(Ray ray, Triangle triangle) {
    normalize(ray.position); normalize(ray.direction);

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
    normalize(ray.position); normalize(ray.direction);

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
        Sphere sphere = Sphere(sphereVectors[i][0].xyz,
                        sphereFloats[i].x,
                        Surface(
                            sphereVectors[i][1].xyz,
                            sphereVectors[i][2].xyz,
                            sphereFloats[i].y,
                            sphereFloats[i].z
                        )
                    );
        HitInfo hit = checkCollision(ray, sphere);

        if (hit.didHit && hit.distance < closestToRay) {
            closestToRay = hit.distance;
            info = hit;
        }
    }

    for (int i = 0; i < numTris; i++) {
        Triangle tri2 = Triangle(
            triangleVectors[i][0].xyz,
            trianglePoints[i][0].xyz,
            trianglePoints[i][1].xyz,
            trianglePoints[i][2].xyz,
            Surface(
                triangleVectors[i][1].xyz,
                triangleVectors[i][2].xyz,
                triangleFloats[i].x,
                triangleFloats[i].y
            )
        );

        HitInfo hit = checkCollision(ray, tri2);
        if (hit.didHit && hit.distance < closestToRay) {
            closestToRay = hit.distance;
            info = hit;
        }
    }

    return info;
}

float randomValue(inout uint seed) {
    seed = seed * 747796405 + uint(2891336453);
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
    uint randomSeed = uint((pixelPosition.y + 1) * 2500) * 500 + uint((pixelPosition.x + 1) * 2500) + iterationCount * 256;

    vec3 pixelCenter = pixel00Pos + (pixelPosition.x * pixelDeltaU) + (pixelPosition.y * pixelDeltaV);

    Ray ray = Ray(vec3(0, 0, 0), vec3(pixelPosition.xy, 2));

    vec3 totalColor = vec3(0);

    for (int i = 0; i < 50; i++) {
        totalColor += traceRay(ray, randomSeed);
    }

    return totalColor / 40;

}


void main() {
    vec3 color = castRays();
    FragColor = (vec4(color, 1)) / 2;
}
