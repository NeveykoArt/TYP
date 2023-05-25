class Program
{
	static int f(int n)
	{
		if (n == 1) {
			int j = n - 1;
			return j;
		}
		if (n == 2) {
			int h = n - 1;
			return h;
		}
		int d = n - 1;
		int e = n - 2;
		int a = f(d);
		int b = f(e);
		int c = a + b;
		return c;
	}

	static int main(string[] args)
	{
		int n;    
		Console.WriteLine("enter the n number of fibonacci");  
		Console.ReadLine(n);       
		int d = f(n);
		Console.WriteLine(d); 
		return 0;
	}
}
