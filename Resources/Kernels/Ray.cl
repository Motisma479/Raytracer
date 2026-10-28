struct
{
	float3 orig;
	float3 dir;
}Ray;

float3 Ray_At(f32 t)
{
	return orig + dir * t;
}