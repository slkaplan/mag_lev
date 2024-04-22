import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass

@dataclass
class MagLevParams:
    # TODO: Make these numbers real
    mass: float = 0.020 # [kg]
    g: float = 9.81     # [m/s^2]
    beta: float = 1e-4  # [N*m^2/V]
    x_eq: float = 0.02  # [m]
    v_eq: float = (mass * g * x_eq**2)/beta     # From equilibrium equation about which we linearized

class MagLevODE:
    def __init__(self, x_eq = 0.01, v_0 = 0.0, del_x_0 = 0.0):
        self.params = MagLevParams()
        self.params.x_eq = x_eq     # Change equilibrium balancing point (depends on chosen v_eq)

        self.ts = [0.0]

        del_v_0 = v_0 - (self.params.mass * self.params.g * self.params.x_eq**2) / self.params.beta
        self.del_vs = [del_v_0]
        self.del_xs = [del_x_0]
        self.del_x_dots = [0.0]    # Assume always starting from rest
        self.state = [del_x_0, 0.0, 0.0, 0.0]   # y = del_x_dot, [del_x, y, del_x_dot, y_dot]

    def sensor(self):
        # Currently functioning as perfect sensor, getting the exact delta x position
        return self.del_xs[-1]

    def update(self, del_t, v_mag):
        # Implementation of Euler's method
        del_v = v_mag - self.params.v_eq
        del_x_dot = self.state[2]
        y_dot = self.state[3]

        del_x = self.state[0] + del_x_dot * del_t
        y = self.state[1] + y_dot * del_t

        self.ts.append(self.ts[-1] + del_t)
        self.del_vs.append(del_v)
        self.del_xs.append(del_x)
        self.del_x_dots.append(y)

        del_x_dot = y
        
        y_dot = (((-self.params.beta) / (self.params.mass * self.params.x_eq**2)) * del_v +
                 ((2 * self.params.g) / (self.params.x_eq**2)) * del_x)        

        self.state = [del_x, y, del_x_dot, y_dot]

    def get_current_state(self):
        return self.ts[-1], self.del_vs[-1], self.state

    def get_all_states(self):
        return self.ts, self.del_vs, self.del_xs, self.del_x_dots
    
class Controller:
    def __init__(self, Kp=0.0, Ki=0.0, Kd=0.0):
        self.Kp = Kp
        self.Ki = Ki
        self.Kd = Kd
        self.e_prev = 0.0
        self.e_total = 0.0

    def command(self, del_t, target_del_x, measured_del_x):
        e = target_del_x - measured_del_x
        P = self.Kp * e
        I = self.Ki * self.e_total * del_t
        D = self.Kd * ((e - self.e_prev)/del_t)
        v = -(P + I + D)
        self.e_total += e
        self.e_prev = e
        return v

if __name__ == "__main__":
    del_t = 0.0001  # [s]
    x_eq = 0.02     # [m]
    v_eq = (MagLevParams.mass * MagLevParams.g * x_eq**2)/MagLevParams.beta
    print(f"{v_eq=}")
    v_0 = v_eq

    K = 300
    Kp = K * 20
    Ki = K * 0
    Kd = K * 1

    controller = Controller(Kp = Kp, Ki = Ki, Kd = Kd)

    sys = MagLevODE(x_eq=x_eq, v_0=v_0, del_x_0=-0.003)
    t_current, _, state = sys.get_current_state()
    del_x = state[0]

    # TODO: replace this number with the actual height of the setup (from electromagnet to base)
    x_max = 0.15

    while x_eq + del_x <= x_max and x_eq + del_x >= 0 and t_current < 8.0:
        v = np.clip(controller.command(del_t, 0, sys.sensor()), -12, 12)
        sys.update(del_t, v)
        t_current, del_v, state = sys.get_current_state()
        del_x = state[0]

    ts, del_vs, del_xs, del_x_dots= sys.get_all_states()
    
    plt.plot(ts, [del_x + x_eq for del_x in del_xs])
    plt.title("MagLev Simulation Results")
    plt.xlabel("Time (s)")
    plt.ylabel("Magnet Stack Position from Top (m)")
    plt.show()