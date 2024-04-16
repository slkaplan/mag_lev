import matplotlib.pyplot as plt
from levitation_sim import MagLevParams

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
    
if __name__ == "__main__":

    del_t = 0.0001  # [s]
    x_eq = 0.01     # [m]
    v_eq = (MagLevParams.mass * MagLevParams.g * x_eq**2)/MagLevParams.beta
    v_0 = 0.1 + v_eq

    sys = MagLevODE(x_eq=x_eq, v_0=v_0)
    _, _, state = sys.get_current_state()
    del_x = state[0]

    # TODO: replace this number with the actual height of the setup (from electromagnet to base)
    x_max = 0.2 

    while del_x <= x_max - x_eq and x_eq + del_x >= 0:
        sys.update(del_t, v_0)
        t, del_v, state = sys.get_current_state()
        del_x = state[0]
        print(f"{t=}, {del_v=}, {del_x=}")

    ts, del_vs, del_xs, del_x_dots= sys.get_all_states()

    plt.plot(ts, del_xs)
    plt.show()