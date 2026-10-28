f32 LinearToGamma(f32 color)
{
	return sqrt(color);
}

float4 RayColor(Ray* ray, s32 maxDepth, )
{

}

__kernel void render(
	__global float4* pixels,
	u32 width,
	u32 height,
	
	float3 cameraCenter,
	float3 pixel00,
	float3 pixelDeltaU,
	float3 pixelDeltaV)
{
	u32 x = get_global_id(0);// replaces your x loop
	u32 y = get_global_id(1);// replaces your y loop
	
	f32 minJitering = -0.5f;
	f32 maxJitering = 0.5f

	u64 pixelIndex = y * width + x;

	RNG rng = rng_create(pixelIndex)

	f32 xJitering = rng_next_float(rng, minJitering, maxJitering);
	f32 yJitering = rng_next_float(rng, minJitering, maxJitering);

	float3 pixelCenter = pixel00 + (pixelDeltaU * (x + xJitering)) + (pixelDeltaV * (y + yJitering));

	float3 rayDirection = pixelCenter - cameraCenter;

	Ray ray = {cameraCenter, rayDirection};
	float4 newColor = (0);// RayColor();

	newColor.r = LinearToGamma(newColor.r);
	newColor.g = LinearToGamma(newColor.g);
	newColor.b = LinearToGamma(newColor.b);

	pixels[pixelIndex] = newColor
}