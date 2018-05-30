library(readr)
library(randomForest)
library(ggplot2)
library(caret)

df <- read_csv("data.csv", col_names = TRUE)
df <- df[-which(df$Steering <= 0 | df$Throttle <= 0), ]
write_csv(df, "data_cleaned.csv")
ggplot(data = df, aes(Throttle)) + geom_histogram(bins = 100)
ggplot(data = df, aes(Steering)) + geom_histogram(bins = 100)

test.index <- createDataPartition(df$Steering, p=0.2, list = FALSE)
df.test <- df[test.index,]
df.train <- df[-test.index,]
#df.test <- read_csv("data_test.csv", col_names = TRUE)
#df.test <- df.test[-which(df.test$Steering < 0 | df.test$Throttle < 0), ]

#Random Forest
#Steering
#Train
model <- randomForest(Steering ~ . - Throttle, data = df.train)
pred <- predict(model, newdata = df.train)
df.comp <- data.frame(Actual = df.train$Steering, Residual = df.train$Steering - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() + 
  ggtitle("Steering (Linear regression Train)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df.train))
print(rmse)

#Steering
#Test
pred <- predict(model, newdata = df.test)
df.comp <- data.frame(Actual = df.test$Steering, Residual = df.test$Steering - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() + 
  ggtitle("Steering (Linear regression Test)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df.test))
print(rmse)

#Throttle
#Train
model <- randomForest(Throttle ~ . - Steering, data = df.train)
pred <- predict(model, newdata = df.train)
df.comp <- data.frame(Actual = df.train$Throttle, Residual = df.train$Throttle - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() + 
  ggtitle("Throttle (RandomForest Train)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df.train))
print(rmse)

#Throttle
#Test
pred <- predict(model, newdata = df.test)
df.comp <- data.frame(Actual = df.test$Throttle, Residual = df.test$Throttle - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() + 
  ggtitle("Throttle (RandomForest Test)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df.test))
print(rmse)



#Linear regression
#Steering
#Train
model <- lm(Steering ~ . - Throttle, data = df.train)
pred <- predict(model, newdata = df.train)
df.comp <- data.frame(Actual = df.train$Steering, Residual = df.train$Steering - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() + 
  ggtitle("Steering (Linear regression Train)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df.train))
print(rmse)

#Steering
#Test
pred <- predict(model, newdata = df.test)
df.comp <- data.frame(Actual = df.test$Steering, Residual = df.test$Steering - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() + 
  ggtitle("Steering (Linear regression Test)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df.test))
print(rmse)

#Throttle
#Train
model <- lm(Throttle ~ . - Steering, data = df.train)
pred <- predict(model, newdata = df.train)
df.comp <- data.frame(Actual = df.train$Throttle, Residual = df.train$Throttle - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() +
  ggtitle("Throttle (Linear regression Train)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df.train))
print(rmse)

#Throttle
#Test
pred <- predict(model, newdata = df.test)
df.comp <- data.frame(Actual = df.test$Throttle, Residual = df.test$Throttle - pred)
ggplot(data = df.comp, mapping = aes(Actual, Residual)) + geom_point() +
  ggtitle("Throttle (Linear regression Test)")
rmse <- sqrt(sum((df.comp$Residual ^ 2))/nrow(df.test))
print(rmse)
