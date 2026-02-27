use std::io;
use std::time::Instant;

fn main()
{
	let n_tau: f64 = 360.0;	// общее время моделирования,				с
	let mut d_tau: f64 = 0.0;
	while (d_tau > n_tau) || (d_tau <= 0.0)
	{
		println!("Введите 0 < Δτ ≤ {} — шаг (с) по времени:", n_tau);
		d_tau = read_input();
	}
	
	let mut d_x: f64 = 0.0;
	while d_x <= 0.0
	{
		println!("Введите Δx (м) > 0 — шаг по пространству:");
		d_x = read_input();
	}
	
	let l: f64 = 0.2222;	// длина пластины,							м
	let n_x = (l / d_x + 1.0) as usize;
	if n_x % 2 == 0
	{
		println!("Ошибка ввода: число шагов чётно или равно одному.");
		return;
	}
	
	let stopwatch_start = Instant::now();
	
	let lambda: f64 = 20.0;	// коэффициент теплопроводности у титана,	Вт / (м × °C)
	let c: f64 = 530.0;		// удельная теплоёмкость для титана,		Дж / (кг × °C)
	let rho: f64 = 4500.0;	// плотность титана,						кг / м³
	
	let mut tmpr = vec![0.0 as f64; n_x];
	tmpr[0] = 1665.0;		// температура на границе x = 0,			°C
	tmpr[n_x - 1] = -273.1;	// температура на границе x = L,			°C
	
	let a = (lambda / d_x.powi(2)) as f64;
	let b = (2.0 * lambda / d_x.powi(2) + rho * c / d_tau) as f64;
	let mut f = (rho * c * tmpr[0] / d_tau) as f64;
	
	let mut tau: f64 = 0.0;
	while tau < n_tau
	{
		let mut alpha = tmpr.clone();
		let mut beta = tmpr.clone();
		
		alpha[1] = a / b;
		beta[1] = f / b;
		
		for i in 2 ..= n_x - 2
		{
			alpha[i] = a / (b - a * alpha[i - 1]);
			f = rho * c * tmpr[i] / d_tau;
			beta[i] = (a * beta[i - 1] + f) / (b - a * alpha[i - 1]);
		}
		
		for i in (1 ..= n_x - 2).rev()
		{
			tmpr[i] = alpha[i] * tmpr[i + 1] + beta[i];
		}
		
		tau += d_tau;
	}
	
	println!("После {} с температура в середине ({} м) пластины равна {} °C.", n_tau, l / 2.0, tmpr[(n_x - 1) / 2]);
	println!("Потребовалось {:.3?}.", stopwatch_start.elapsed());
}

fn read_input() -> f64
{
	let mut input = String::new();
	io::stdin().read_line(&mut input).expect("Ошибка ввода.");
	input.trim().parse().expect("Ошибка преобразования.")
}
