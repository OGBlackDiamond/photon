#version 460 core

out vec4 FragColor;
in vec2 pixelPosition;

uniform vec3 pixel00Pos;
uniform vec3 pixelDeltaU;
uniform vec3 pixelDeltaV;



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

    HitInfo hit1 = checkCollision(ray, sphere1);
    if (hit1.didHit) {
        info = hit1;
    }
    HitInfo hit2 = checkCollision(ray, sphere2);
    if (hit2.didHit) {
        if (hit1.didHit) {
            if (hit1.distance < hit2.distance) {
                info = hit2;
            }
        } else {
            info = hit2;
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

    for (int i = 0; i <= 30; i++) {
        HitInfo hit = calculateRayCollisions(ray);
        if (hit.didHit) {
            vec3 randomDir = normalize(hit.normal + randomDirection(randomSeed));
            
            ray.position = hit.hitPoint;
            ray.direction = randomDir;

            Surface surface = hit.shapeSurface;

            light += color * (surface.emissionColor * surface.emissionStrength);
            color *= surface.surfaceColor;
        } else break;
    }

    return light;
}


vec3 castRays() {
    uint randomSeed = uint((pixelPosition.y + 1) * 2500) * 500 + uint((pixelPosition.x + 1) * 2500);

    vec3 pixelCenter = pixel00Pos + (pixelPosition.x * pixelDeltaU) + (pixelPosition.y * pixelDeltaV);

    Ray ray = Ray(vec3(0, 0, 0), vec3(pixelPosition.xy, 2));

    vec3 totalColor = vec3(0);

    for (int i = 0; i < 1000; i++) {
        totalColor += traceRay(ray, randomSeed);
    }

    return totalColor / 1000;

}


void main() {
    FragColor = vec4(castRays(), 1);
}
