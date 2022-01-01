__kernel void Bitonic_sort(	__global uint * INPUT,
							const uint stage_no,
							const uint current_pass,
							uint dir
							)
{
	// Get the index of the current element to be processed
	uint i = get_global_id(0);
	
	uint shift_ammount = stage_no - current_pass;
	uint distance = 1 << shift_ammount;
	uint left_mask = distance - 1;
	uint direction_mask = 1 << stage_no;

	uint left_index = ((i >> shift_ammount) << (shift_ammount + 1)) + (i & left_mask);
	uint right_index = left_index + distance;

	uint left = INPUT[left_index];
	uint right = INPUT[right_index];
	uint bigger, smaller;

	if (left > right)
	{
		bigger = left;
		smaller = right;
	}
	else
	{
		bigger = right;
		smaller = left;
	}

	if ((i & direction_mask) == direction_mask)
		dir = !dir;

	if (dir)
	{
		INPUT[left_index] = smaller;
		INPUT[right_index] = bigger;
	}
	else
	{
		INPUT[left_index] = bigger;
		INPUT[right_index] = smaller;
	}
}
