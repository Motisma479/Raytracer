f32 add(f32 a, f32 b)
{
	return a+b;
}
__kernel void hello_kernell(
	__global f32 *a,
	__global f32 *b,
	__global f32 *result)
{
	u32 i = get_global_id(0);
	result[i] = add(a[i],b[i]);
}