class Program {
	static void function1(int i) {
		Console.WriteLine("chetnoe");
		Console.WriteLine(i);
		return;
	}
	
	static void function2(int i) {
		Console.WriteLine("nechetnoe");
		Console.WriteLine(i);
		return;
	}
	
	static int main(string[] args) {
		for (int i = 0; i < 10; i++) {
			int n = i % 2;
			if (n == 0) {
				function1(i);
			} else {
				function2(i);
			}
		}
		
		return 0;
	}
}
