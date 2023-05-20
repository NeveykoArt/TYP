class Program
{
	static int sum(int a, int b) {
		int c = a + b;
		return c;
	}
	int e = 2;
	int[] f;
	int[] g = new int[5];
	int[] h = new int[5]{1,2,3,4,5};
	static void Main(string[] args)
	{
		int a = 2 + 3;
		int b = 2;
		int c = 0;
		bool d = true;
		Console.WriteLine("Hello, World!");
		if (a == b) {
			c = sum(a, b);
		} else {
			Console.WriteLine("hahahahaha");
		}
		for (int i = 0; i < a; i++) {
			Console.WriteLine("aboba");
		}
		for (int i = 0; i < 5; i++) {
			int j = h[i];
			Console.WriteLine(j);
		}
		Console.WriteLine(c);
		return;
	}
}
