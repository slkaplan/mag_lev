import numpy as np
import matplotlib.pyplot as plt
import control
from levitation_sim import MagLevParams

if __name__ == "__main__":
    p = MagLevParams()
    G_s = control.tf([-p.beta/(p.mass * p.x_eq**2)], [1, 0, (-2 * p.g) / p.x_eq])

    t, X = control.step_response(G_s)

    plt.plot(t, X)
    plt.show()