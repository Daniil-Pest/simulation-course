sample_size <- 100000

# Мультипликативный конгруэнтный генератор
a <- 16807 # Классический множитель для МКГ
m <- 2^31 - 1 # 2147483647; большое простое число
seed <- 1
lcg_values <- numeric(sample_size) # Вектор для значений
current <- seed
for (i in 1:sample_size)
{
  current <- (a * current) %% m # Остаток от деления
  lcg_values[i] <- current / m  # Нормализация в [0, 1)
}

# Встроенный генератор R
set.seed(1)
r_values <- runif(sample_size, min = 0, max = 1) # Генерация из равномерного распределения

# Функции для вычисления среднего и дисперсии
mean <- function(x) sum(x) / length(x)
variance <- function(x) sum((x - mean(x))^2) / (length(x) - 1)

# Результаты для МКГ
lcg_mean <- mean(lcg_values)
lcg_variance <- variance(lcg_values)

cat("Мультипликативный конгруэнтный генератор\n")
cat(sprintf("Среднее: %.6f\n", lcg_mean))
cat(sprintf("Дисперсия: %.6f\n\n", lcg_variance))

# Результаты для встроенного генератора R
r_mean <- mean(r_values)
r_variance <- variance(r_values)

cat("Встроенный генератор R\n")
cat(sprintf("Среднее: %.6f\n", r_mean))
cat(sprintf("Дисперсия: %.6f\n\n", r_variance))
