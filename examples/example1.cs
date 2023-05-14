class Program
{
	int sum(int a, int b) {
		int c = a + b;
		return c;
	}
	int a = 2;
	float b = 2.0;
	char c = 'a';
	string d = "hello";
	bool e = true;
	bool f = false;
	a = 3;
	b = 3.0;
	c = 'b';
	d = "goodbye";
	e = false;
	f = true;
	int[] h;
	h = new int[2] {1, 2};
	int g = h[2];
	g = a;
	a = sum(1, 2);
}
