#version 460 core

out vec4 FragColor;
in vec2 vertexPosition;


struct Ray {
    vec3 position;
    vec3 direction;
};

vec3 sphereCenter = vec3(0, 0, 5);
float sphereRadius = 1;

bool checkCollision(Ray ray, vec3 center, float radius) {
    normalize(ray.position); normalize(ray.direction);

    vec3 oc = center - ray.position;
    
    float a = pow(length(ray.direction), 2);
    float b = dot(oc, ray.direction);
    float c = pow(length(oc), 2) - pow(radius, 2);

    float dis = b * b - a * c;

    return dis >= 0;
}

void main() {
    Ray ray = Ray(vec3(0, 0, 0), vec3(vertexPosition.xy, 2));
    if (checkCollision(ray, sphereCenter, sphereRadius)) {
        FragColor = vec4(1, 1, 1, 1);
    } else {
        FragColor = vec4(0, 0, 0, 1);
    }
}
