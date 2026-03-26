use rand::Rng;

fn main()
{
	// Настройка генераторов
	let mut rng_thread = rand::thread_rng();
	let mut lcg = LCG::new(12345, 1103515245, 12345, 1664525);
	
	struct LCG
	{
		state: u32,
		a: u32,
		c: u32,
		m: u32,
	}
	
	impl LCG
	{
		fn new(seed: u32, a: u32, c: u32, m: u32) -> Self
		{
			LCG
			{
				state: seed, a, c, m  
			}
		}
	
		fn next(&mut self) -> u32
		{
			self.state = (self.a * self.state + self.c) % self.m;
			self.state
		}
	}
	
	// Функция для вычисления выборочного среднего и дисперсии
	fn calculate_stats(data: &[f64]) -> (f64, f64)
	{
		let mean = data.iter().sum::<f64>() / data.len() as f64;
		let variance = data.iter().map(|x| (x - mean).powi(2)).sum::<f64>() / data.len() as f64;
		(mean, variance)
	}
	
	// Генерация выборок
	let sample_size = 100000;
	let mut thread_rng_samples = vec![0.0; sample_size];
	let mut lcg_samples = vec![0.0; sample_size];
	
	for i in 0..sample_size
	{
		thread_rng_samples[i] = rng_thread.gen::<f64>();
		lcg_samples[i] = lcg.next() as f64 / lcg.m as f64;
	}
	
	let (thread_mean, thread_variance) = calculate_stats(&thread_rng_samples);
	let (lcg_mean, lcg_variance) = calculate_stats(&lcg_samples);
	
	println!("Выборочное среднее (thread_rng): {}", thread_mean);
	println!("Выборочная дисперсия (thread_rng): {}", thread_variance);
	println!("Выборочное среднее (LCG): {}", lcg_mean);
	println!("Выборочная дисперсия (LCG): {}", lcg_variance);
}
