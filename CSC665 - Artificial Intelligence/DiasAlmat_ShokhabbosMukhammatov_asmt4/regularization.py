#------------------------------------
# Author: T. D. Devlin
#-----------------------------------

import math
from math import sin, pi
from random import random


def f(x):
    return sin(pi * x)


def generate_training_examples(n=2):
    xs = [random() * 2 - 1 for _ in range(n)]
    return [(x, f(x)) for x in xs]


def fit_without_reg(examples):
    """Computes values of w0 and w1 that minimize the sum-of-squared-errors cost function.

    For exactly two data points there is a unique line passing through both,
    so we solve for w0 and w1 directly (no gradient descent required):

        w1 = (y2 - y1) / (x2 - x1)   <-- slope
        w0 = y1 - w1 * x1             <-- intercept

    Args:
    - examples: a list of two (x, y) tuples, where x is the feature and y is the label
    """
    w0 = 0
    w1 = 0
    ## BEGIN YOUR CODE ##

    (x1, y1), (x2, y2) = examples

    if x1 == x2:
        # Degenerate case (negligible probability when sampling uniformly):
        # return a constant prediction equal to the average of the two labels.
        w0 = (y1 + y2) / 2.0
        w1 = 0.0
    else:
        w1 = (y2 - y1) / (x2 - x1)
        w0 = y1 - w1 * x1

    ## END YOUR CODE ##
    return w0, w1


def fit_with_reg(examples, lambda_hp):
    """Computes values of w0 and w1 that minimize the regularized
    sum-of-squared-errors cost function using gradient descent.

    Augmented cost:
        C_tilde(w) = (y1-(w0+w1*x1))^2 + (y2-(w0+w1*x2))^2
                     + lambda*(w0^2 + w1^2)

    Gradients:
        dC~/dw0 = -2*(y1-(w0+w1*x1)) - 2*(y2-(w0+w1*x2)) + 2*lambda*w0
        dC~/dw1 = -2*x1*(y1-(w0+w1*x1)) - 2*x2*(y2-(w0+w1*x2)) + 2*lambda*w1

    Gradient descent update rule:  w <- w - eta * grad
    with eta = 0.05, 1000 iterations, starting from w0 = w1 = 0.

    Args:
    - examples: a list of two (x, y) tuples, where x is the feature and y is the label
    - lambda_hp: a float representing the value of the lambda hyperparameter;
                 a larger value means more regularization
    """
    w0 = 0.0
    w1 = 0.0
    ## BEGIN YOUR CODE ##

    eta = 0.05        # step size (given)
    num_iters = 1000  # number of gradient-descent updates (given)

    (x1, y1), (x2, y2) = examples

    for _ in range(num_iters):
        r1 = y1 - (w0 + w1 * x1)   # residual for point 1
        r2 = y2 - (w0 + w1 * x2)   # residual for point 2

        grad_w0 = -2 * r1 - 2 * r2 + 2 * lambda_hp * w0
        grad_w1 = -2 * x1 * r1 - 2 * x2 * r2 + 2 * lambda_hp * w1

        w0 -= eta * grad_w0
        w1 -= eta * grad_w1

    ## END YOUR CODE ##
    return (w0, w1)


def test_error(w0, w1):
    n = 100
    xs = [i/n for i in range(-n, n + 1)]
    return sum((w0 + w1 * x - f(x)) ** 2 for x in xs) / len(xs)


if __name__ == "__main__":

    ## BEGIN YOUR SIMULATION CODE ##

    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    import numpy as np

    num_trials = 1000
    lambda_val = 1.0

    total_error_no_reg   = 0.0
    total_error_with_reg = 0.0

    # Store (w0, w1) from every trial for the EC plot (Part 6)
    params_no_reg   = []
    params_with_reg = []

    for _ in range(num_trials):
        examples = generate_training_examples(n=2)

        w0_nr, w1_nr = fit_without_reg(examples)
        w0_r,  w1_r  = fit_with_reg(examples, lambda_hp=lambda_val)

        total_error_no_reg   += test_error(w0_nr, w1_nr)
        total_error_with_reg += test_error(w0_r,  w1_r)

        params_no_reg.append((w0_nr, w1_nr))
        params_with_reg.append((w0_r,  w1_r))

    avg_error_no_reg   = total_error_no_reg   / num_trials
    avg_error_with_reg = total_error_with_reg / num_trials

    print("Average test error WITHOUT regularization : {:.4f}".format(avg_error_no_reg))
    print("Average test error WITH regularization    : {:.4f}".format(avg_error_with_reg))

    # -------------------------------------------------------------------
    # EC Part 6 -- plot all 1000 fitted lines alongside the target function
    # -------------------------------------------------------------------
    xs_plot = np.linspace(-1, 1, 300)
    f_vals  = np.sin(np.pi * xs_plot)

    fig, axes = plt.subplots(1, 2, figsize=(12, 5), sharey=True)

    panel_info = [
        ("Without Regularization",                params_no_reg),
        ("With Regularization (lambda=1)",         params_with_reg),
    ]

    for ax, (title, params) in zip(axes, panel_info):
        # Draw each of the 1000 hypothesis lines (thin, semi-transparent)
        for w0, w1 in params:
            ax.plot(xs_plot, w0 + w1 * xs_plot,
                    color='steelblue', linewidth=0.4, alpha=0.07)

        # Overlay the target function in red
        ax.plot(xs_plot, f_vals, color='red', linewidth=2.0,
                label='f(x) = sin(pi*x)')

        ax.set_xlim(-1, 1)
        ax.set_ylim(-3, 3)
        ax.set_xlabel('x', fontsize=12)
        ax.set_ylabel('y', fontsize=12)
        ax.set_title(title, fontsize=13)
        ax.legend(loc='upper right')

    plt.suptitle("Linear Regression Fits Over 1000 Trials", fontsize=14)
    plt.tight_layout()
    plt.savefig('regularization_plot.png', dpi=150)
    print("Plot saved -> regularization_plot.png")

    ## END YOUR SIMULATION CODE ##
