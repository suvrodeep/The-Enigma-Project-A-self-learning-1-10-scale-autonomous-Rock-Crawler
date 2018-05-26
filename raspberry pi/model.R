library(readr)
library(randomForest)
library(ggplot2)

df <- read_csv("data.csv", col_names = TRUE)
df <- df[-which(df$Steering < 0 | df$Throttle < 0), ]
write_csv(df, "data_cleaned.csv")

df.test <- read_csv("data_test.csv", col_names = TRUE)
df.test <- df.test[-which(df.test$Steering < 0 | df.test$Throttle < 0), ]

#Random Forest
#Steering
#Train
model <- randomForest(Steering ~ . - Throttle, data = df)
pred <- predict(model, newdata = df)
df.comp <- data.frame(Actual = df$Steering, Residual = df$Steering - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() + 
  ggtitle("Steering (RandomForest Train)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df))
print(rmse)

#Steering
#Test
pred <- predict(model, newdata = df.test)
df.comp <- data.frame(Actual = df.test$Steering, Residual = df.test$Steering - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() + 
  ggtitle("Steering (RandomForest Test)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df))
print(rmse)

#Throttle
#Train
model <- randomForest(Throttle ~ . - Steering, data = df)
pred <- predict(model, newdata = df)
df.comp <- data.frame(Actual = df$Throttle, Residual = df$Throttle - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() + 
  ggtitle("Throttle (RandomForest Train)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df))
print(rmse)

#Throttle
#Test
pred <- predict(model, newdata = df.test)
df.comp <- data.frame(Actual = df.test$Throttle, Residual = df.test$Throttle - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() + 
  ggtitle("Throttle (RandomForest Test)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df))
print(rmse)



#Linear regression
model <- lm(Steering ~ . - Throttle, data = df)
pred <- predict(model, newdata = df)
df.comp <- data.frame(Actual = df$Steering, Residual = df$Steering - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point()
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df))

model <- lm(Throttle ~ . - Steering, data = df)
pred <- predict(model, newdata = df)
df.comp <- data.frame(Actual = df$Throttle, Residual = df$Throttle - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point()
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df))

