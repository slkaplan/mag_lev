import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass

@dataclass
class MagLevParams:
    # TODO: Make these numbers real
    mass: float = 0.001 # [kg]
    g: float = 9.81     # [m/s^2]
    beta: float = 1e-6   # [N*m^2/V]
    x_eq: float = 0.01  # [m]
    v_eq: float = (mass * g * x_eq**2)/beta     # From equilibrium equation about which we linearized

class MagLev:
    def __init__(self, x_eq = 0.01, v_0 = 0.0, del_x_0 = 0.0, del_t_0 = 0.0001):
        self.params = MagLevParams()
        self.params.x_eq = x_eq     # Change equilibrium balancing point (depends on chosen v_eq)
        self.ts = [0.0, del_t_0]
        del_v_0 = v_0 - (self.params.mass * self.params.g * self.params.x_eq**2) / self.params.beta
        self.del_vs = [del_v_0, del_v_0]
        self.del_xs = [del_x_0, del_x_0]

    def get_hall_effect(self):
        # TODO: Return a hall effect V_out reading based on the current state of the system (magnet position)
        pass
    
    def update(self, del_t, v_mag):
        self.del_vs.append(v_mag - self.params.v_eq)
        self.del_xs.append(2 * self.del_xs[-2] - self.del_xs[-1] + 
                           (((-self.params.beta) / (self.params.mass * self.params.x_eq**2)) * self.del_vs[-1] + 
                            ((2 * self.params.g) / (self.params.mass * self.params.x_eq)) * self.del_xs[-1]) * del_t**2)
        self.ts.append(self.ts[-1] + del_t)
    
    def get_current_state(self):
        return self.ts[-1], self.del_vs[-1], self.del_xs[-1]

    def get_all_states(self):
        return self.ts, self.del_vs, self.del_xs

class Controller:
    def __init__(self, Kp, Ki, Kd):
        self.Kp = Kp
        self.Ki = Ki
        self.Kd = Kd
        self.e_prev = 0.9
        self.e_total = 0.0

    def command(self, target_del_x, measured_del_x):
        e = target_del_x - measured_del_x
        v = self.Kp * e + self.Ki * self.e_total + self.Kd * (self.e - self.e_prev)
        self.e_total += e
        self.e_prev = e
        return v

if __name__ == "__main__":

    del_t = 0.0001  # [s]
    x_eq = 0.01     # [m]
    sys = MagLev(x_eq=x_eq)
    _, _, del_x= sys.get_current_state()

    # TODO: replace this number with the actual height of the setup (from electromagnet to base)
    x_max = 0.2 

    while del_x <= x_max - x_eq and x_eq + del_x >= 0:
        sys.update(del_t, 0.2)
        t, del_v, del_x= sys.get_current_state()
        print(f"{t=}, {del_v=}, {del_x=}")

    ts, del_vs, del_xs = sys.get_all_states()

    plt.plot(ts, del_xs)
    plt.show()