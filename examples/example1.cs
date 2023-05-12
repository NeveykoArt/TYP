class Program
{
	int sum(int a, int b) {
		int c = a + b;
		return c;
	}
	
	static void Main(string[] args)
	{
		int a = 2;
		int b = 2;
		int c;
		WriteLine("Hello, World!");
		if (a == b) {
			c = sum(a, b);
		}
		WriteLine(c);
		return 0;
	}
}
