lambda <- 5.0 # Интенсивность потока
T <- 10.0     # Интервал времени
N <- 1000     # Число экспериментов

# Функция для симуляции пуассоновского потока за интервал T
simulate_poisson <- function(lambda, T)
{
  current_time <- 0
  count <- 0

  while (TRUE)
  {
    interval <- rexp(1, rate = lambda) # Из экспоненциального распределения
    current_time <- current_time + interval # Генерация времени до следующего события

    if (current_time > T)
    {
        break
    }

    count <- count + 1
  }

  return(count)
}

# Проводим N экспериментов
results <- sapply(1:N, function(x) simulate_poisson(lambda, T))

# Среднее и дисперсия
mean_value <- mean(results)
variance_value <- var(results)

cat("Среднее число запросов за интервал T:", mean_value, "\n")
cat("Дисперсия числа запросов за интервал T:", variance_value, "\n")

# Гистограмма эмпирического распределения
hist(results,
     main = "Эмпирическое распределение числа запросов",
     xlab = "Число запросов за интервал T",
     ylab = "Частота",
     breaks = seq(min(results), max(results) + 1, by = 1),
     col = "lightblue",
     border = "black")
# Вертикальная линия для среднего значения
abline(v = mean_value, col = "red", lwd = 2, lty = 2)
legend("topright", legend = c(paste("Среднее:", round(mean_value, 2))), col = "red", lty = 2, lwd = 2)
