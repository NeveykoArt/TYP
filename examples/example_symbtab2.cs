class Program
{
	static int main(string[] args)
	{
		int[] a;
		a = new int[5]{1, 2, 3, 4, 5};
		
		for (int i = 0; i < 5; i++) {
			Console.WriteLine(a[i]);
		}
		
		a[2] = 10;
		
		int b = 5;
		a[3] = 10 + b;
		
		int c = 7;
		a[4] = c + b;
		
		for (int i = 0; i < 5; i++) {
			Console.WriteLine(a[i]);
		}
		
		return 0;
	}
}
