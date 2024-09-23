int max(int x, int y) {
	return (x > y) ? x : y;
}

int min(int x, int y) {
	return (x < y) ? x : y;
}

float crossProduct(int v1[], int v2[])
{
        return v1[0]*v2[1] - v1[1]*v2[0];
}