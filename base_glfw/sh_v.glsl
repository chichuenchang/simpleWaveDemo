#version 330 core
//[uniform]
uniform vec3 mousPck;
uniform float uTime;
//mat4
uniform mat4 xform;			
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
//wave variables
uniform vec2 WvDir;
uniform float Amp;
uniform float WvLen;
uniform float Spd;
uniform float StpQ;
uniform int WvMode;

//[attribute]
layout(location = 0) in vec3 pos;		
layout(location = 2) in vec2 texCoord;		

//[vafying]
//smooth out vec3 fragNorm;	
out vec2 vsOut_uv;
smooth out vec3 vsOut_normal_w;
out vec3 vsOut_wavePos_w;

//[local]
//test=====================================
//vec2 d = vec2(1.0f, 1.0f);
//float testDirW = 1.0f* sin( dot(WvDir , pos.xz) * (2.0f / 2.0f)+ uTime * 1.0f *( 2.0f/ 2.0f));
//vec2 d1 = (pos.xz - WvDir) / length(pos.xz - WvDir) ;
//float testCirW = 1.0f* sin( -dot(d1, pos.xz) * (2.0f / 2.0f) + uTime * 1.0f *( 2.0f/ 2.0f));
//test================================

//gerstner wave
float pi = 3.1415926; 
vec4 normal_w;
//float FreqW = 2.0f * pi / WvLen;
//float Phi = Spd * 2.0f * pi / WvLen;
//
//vec3 GerstnerDisp = vec3(StpQ * Amp * normalize(WvDir.x)* cos(FreqW* dot(normalize(WvDir), pos.xz) + Phi * uTime),
//						Amp * sin(FreqW* dot(normalize(WvDir), pos.xz) +  Phi*uTime),
//						StpQ * Amp * normalize(WvDir.y)* cos(FreqW* dot(normalize(WvDir), pos.xz) + Phi * uTime));
//

vec3 Gerstner(vec2 direction, float amplitude, float waveLength, float speed, float steepness){

	float FreqW = 2.0f *pi  / waveLength;
	float Phi = speed * 2.0f  / waveLength;

	if (WvMode==1){
		direction = (direction - pos.xz) / length( direction- pos.xz );
	}

	vec3 GerstnerWave = vec3(steepness * amplitude * normalize(direction.x)*cos(FreqW* dot(normalize(direction), pos.xz) + Phi * uTime),
						amplitude * sin(FreqW* dot(normalize(direction), pos.xz) +  Phi*uTime),
						steepness * amplitude *normalize(direction.y)* cos(FreqW* dot(normalize(direction), pos.xz) + Phi * uTime));

	return GerstnerWave;
}

vec3 normalGerstner (vec2 direction, float amplitude, float waveLength, float speed, float steepness){
	float FreqW = 2.0f *pi / waveLength;
	float Phi = speed * 2.0f / waveLength;

	if (WvMode==1){
		direction = (direction - pos.xz) / length( direction- pos.xz );
	}

	float WA = FreqW * amplitude;
	float S = sin(FreqW * dot(normalize(direction), pos.xz) + Phi*uTime);
	float C = cos(FreqW * dot(normalize(direction), pos.xz) + Phi*uTime);
	//when adding normal, be aware of the y component
	vec3 normal = vec3(-normalize(direction.x) * WA * C, 1 - steepness * WA * S, -normalize(direction.y) * WA * C) ;

	return normal;
}


void main() {
	//test 
	//gl_Position = xform * vec4(pos + vec3(0.0f, testDirW, 0.0f), 1.0);
	
	//Gerstner
	//gl_Position = xform * vec4(pos + GerstnerDisp, 1.0);
	vec3 wavePos_m = pos + Gerstner(WvDir, Amp, WvLen, Spd, StpQ);
	vec4 wavePos_w = M*vec4(wavePos_m, 1.0f);
	vec4 gridPos_w = M*vec4(pos, 1.0f);

	//GerstnerNormal
	normal_w =  M * vec4 ( normalGerstner (WvDir, Amp, WvLen, Spd, StpQ), 0.0f);


	//island1 cube
	if ((wavePos_m.x >= -10.0f && wavePos_m.x <= -7.0f)&& (wavePos_m.z >= 9.0f && wavePos_m.z <= 12.0f)){
		wavePos_m = vec3(wavePos_m.x, 4.0f, wavePos_m.z);
		//wavePos_w = gridPos_w;
		normal_w = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	}
	//island cylinder
	if (length(wavePos_m.xz- vec2( - 0.0f, -0.0f )) <= 4.0f){
		wavePos_m = vec3(wavePos_m.x, 4.0f, wavePos_m.z);
		//wavePos_w = gridPos_w;
		normal_w = vec4(0.0f, 1.0f, 0.0f, 0.0f);

	}
	//island cylinder
	if (length(wavePos_m.xz- vec2( 10.0f, 9.0f )) <= 5.0f){
		wavePos_m = vec3(wavePos_m.x, 4.0f, wavePos_m.z);
		normal_w = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	}

	gl_Position = xform * vec4(wavePos_m, 1.0f);
	//gl_Position = xform * vec4(pos, 1.0f);


	//[varying]
	vsOut_wavePos_w = vec3(M*vec4(wavePos_m, 1.0f)).xyz;
	vsOut_uv = texCoord;
	//normal_w =  M * vec4 ( normalGerstner (WvDir, Amp, WvLen, Spd, StpQ), 0.0f);
	vsOut_normal_w = normalize(vec3(normal_w.xyz));

}