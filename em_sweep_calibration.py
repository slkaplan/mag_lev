import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit



x = np.linspace(0, 99, num = 100)

y_list = []

file1 = open('em_sweep.txt', 'r')
while True:
    # Get next line from file
    line = file1.readline()

    # break if no new data
    if not line:
        break
    y_list.append(line.rstrip('\n'))
 
    
file1.close()

y = np.array(y_list)

# plotting raw data
fig, ax = plt.subplots()
ax.plot(x,y)
ax.invert_yaxis()
plt.xlabel("Electromagnet PWM Pulse")
plt.ylabel("Hall Effect Sensor Raw Data")
plt.title("Electromagnet Sweep Calibration")

ax.set_yticks(ax.get_yticks()[::10])

plt.show()  # display


# curve_fit() function takes the test-function
# x-data and y-data as argument and returns 
# the coefficients a and b in param and
# the estimated covariance of param in param_cov
param, param_cov = curve_fit(x, y)


print("Sine function coefficients:")
print(param)
print("Covariance of coefficients:")
print(param_cov)

# ans stores the new y-data according to 
# the coefficients given by curve-fit() function
ans = (param[0]*(np.sin(param[1]*x)))

'''Below 4 lines can be un-commented for plotting results 
using matplotlib as shown in the first example. '''

# plt.plot(x, y, 'o', color ='red', label ="data")
# plt.plot(x, ans, '--', color ='blue', label ="optimized data")
# plt.legend()
# plt.show()
