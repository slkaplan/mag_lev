import numpy as np
import matplotlib.pyplot as plt
import control
from levitation_ode_sim import MagLevParams

if __name__ == "__main__":
    p = MagLevParams()
    K = 1000
    Kp = K*30
    Ki = K*0
    Kd = K*1
    G_s = control.tf([-p.beta/(p.mass * p.x_eq**3)], [1, 0, (-3 * p.g) / p.x_eq])
    C_s = control.tf([Kd, Kp], [1])

    G_CL = (C_s * G_s)/(1 + C_s * G_s)

    t, X = control.step_response(G_CL)

    plt.plot(t, X)
    plt.show()