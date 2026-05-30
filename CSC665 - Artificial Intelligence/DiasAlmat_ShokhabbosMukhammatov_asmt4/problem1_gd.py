import numpy as np

# Training data: columns are x1, x2
X = np.array([
    [-4,  0],
    [-1,  1],
    [ 0, -1],
    [ 2,  1],
    [ 3,  0],
    [ 6, -1]
], dtype=float)

y = np.array([-1, 1, -1, 1, 1, -1], dtype=float)

# Add bias column for w0
X_aug = np.column_stack((np.ones(X.shape[0]), X))

# Initialize weights w0, w1, w2
w = np.zeros(3)

eta = 0.1
tol = 1e-5
max_iters = 100000

for iteration in range(max_iters):
    grad = np.zeros(3)

    # Compute full gradient using entire dataset
    for i in range(len(y)):
        h = np.dot(w, X_aug[i])
        margin = y[i] * h

        # Hinge loss subgradient
        if margin < 1:
            grad += -y[i] * X_aug[i]

    # Average gradient over dataset
    grad = grad / len(y)

    old_w = w.copy()

    # Gradient descent update
    w = w - eta * grad

    # Check convergence
    if np.linalg.norm(w - old_w) < tol:
        break

# Compute predictions
scores = X_aug @ w
predictions = np.where(scores >= 0, 1, -1)

# Training error
training_error = np.mean(predictions != y)

print("Batch Gradient Descent Results")
print("------------------------------")
print(f"Iterations: {iteration + 1}")
print(f"Final weights: w0={w[0]:.6f}, w1={w[1]:.6f}, w2={w[2]:.6f}")
print(f"Predictions: {predictions}")
print(f"Training error: {training_error:.6f}")

print("\nSGD comparison from Question 3:")
print("SGD final weights: w0=-0.1, w1=-0.1, w2=0.4")
print("SGD training error: 0.333333")