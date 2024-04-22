import numpy as np
import matplotlib.pyplot as plt
import control
from levitation_ode_sim import MagLevParams

if __name__ == "__main__":
    p = MagLevParams()
    Kp = 1000
    Ki = 0
    Kd = 10
    G_s = control.tf([-p.beta/(p.mass * p.x_eq**2)], [1, 0, (-2 * p.g) / p.x_eq])
    C_s = control.tf([Kd, Kp], [1])

    # control.root_locus(C_s*G_s)

    G_CL = (C_s * G_s)/(1 + C_s * G_s)

    t, X = control.step_response(G_CL)

    plt.plot(t, X)
    plt.show()