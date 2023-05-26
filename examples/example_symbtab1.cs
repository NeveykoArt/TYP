class Program
{
	static int fibonacci(int n)
	{
		if (n == 1) {
			int j = n - 1;
			return j;
		}
		int d = n - 1;
		int e = n - 2;
		int a = fibonacci(d);
		int b = fibonacci(e);
		int c = a + b;
		return c;
	}

	static int main(string[] args)
	{
		int n;
		Console.WriteLine("enter the n number of fibonacci");
		Console.ReadLine(n);
		int d = fibonacci(n);
		Console.WriteLine(d);
		return 0;
	}
}
