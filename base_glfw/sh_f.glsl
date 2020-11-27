#version 330 core
//[uniform]
//uniform sampler2D texUnit;
uniform vec3 mousPck;
uniform float uTime;
uniform mat4 M;
uniform bool debugSwtch;

//[varying]
in vec2 vsOut_uv;
in vec3 vsOut_wavePos_w;
smooth in vec3 vsOut_normal_w;

//[out]
out vec4 outCol;	

//[local]
vec3 lightPos = vec3(0.0f, 2.0f, 0.0f);
vec3 camVec = vec3 (0.0f, 0.0f, -1.0f);

void main() {
	//lighting
	vec4 light_vec = M*vec4(lightPos, 1.0f)  - vec4(vsOut_wavePos_w, 1.0f);

	vec3 n = normalize(vsOut_normal_w);
	vec3 l = normalize(light_vec.xyz);
	vec3 r = reflect(-l, n);
	vec3 v = normalize ( (vec4(camVec,0.0f)).xyz);
	//ambient
	vec4 ambientCol = vec4(0.05f, 0.05f, 0.1f, 1.0f);
	//lambertion
	vec4 diffMat = vec4(0.3f, 0.5f, 0.9f, 1.0f); //water albedo
	vec4 litIntens = vec4 (0.6f, 0.6f, 0.6f, 1.0f);
	vec4 lamberCol =1.8f* litIntens* diffMat * max(0.0f, dot(n, l));
	//phong
	vec4 phongCol = 0.6f* litIntens * pow(max(0.0f, dot(r, v)), 7);

	if (!debugSwtch) {
	outCol = ambientCol + lamberCol + phongCol ;
	}
	else{
	outCol = vec4(n, 1.0);

	}

	//outCol = vec4(vsOut_uv, 0.0, 1.0);
	//outCol = vec4(vsOut_uv, 0.9f-sin(uTime), 1.0f) * 0.5f + vec4(0.5f);
	//outCol = vec4(n, 1.0f) ;
	
	//[debug section]
	//outCol = vec4(debugSwtch, 0.0f, 0.0f, 1.0);

}