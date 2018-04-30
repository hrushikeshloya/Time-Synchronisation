## Time synchronization

from scipy.signal import butter, lfilter, freqz
import pylab as plt
import numpy as np
from scipy import signal

def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a

def butter_lowpass_filter(data, cutoff, fs, order=5):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)
    return y


## Low pass filter specs
order = 6
fs = 2500.0       # sample rate, Hz
cutoff = 25.0  # desired cutoff frequency of the filter, Hz

# Get the filter coefficients so we can check its frequency response.
b, a = butter_lowpass(cutoff, fs, order)


f = open('mode1-ex8')
# f_arr = f.read().split('\n')[9:-21]
# f_arr = f.read().split('\n')[20:-21] ##for ex4 
# f_arr = f.read().split('\n')[22:-21]  ##for ex5

f_arr = f.read().split('\n')[17:-21] ##for ex6


### FOR S2

fs1_x = []
fs1_y = []
fs1_z = []

fs2_x = []
fs2_y = []
fs2_z = []


#### Without Timesync analysis ####


iterations = 0
for i in f_arr:
	iterations += 1
	if(iterations <= 5):
		if "s1" in i:
			fs1 = i[2:-6].replace('&&&','')
			# print fs1
			# print offset
			for j in range(len(fs1)):
				if(j%3 == 0):
					fs1_x.append(ord(fs1[j]))
				elif(j%3 == 1 ):
					fs1_y.append(ord(fs1[j]))
				else:
					fs1_z.append(ord(fs1[j]))
			
		elif "s2" in i:
			fs2 = i[2:-6].replace('&&&','')
			#print fs2
			#print offset
			for j in range(len(fs2)):
				if(j%3 == 0):
					fs2_x.append(ord(fs2[j]))
				elif(j%3 == 1 ):
					fs2_y.append(ord(fs2[j]))
				else:
					fs2_z.append(ord(fs2[j]))
	
	### Offset calculation
	if(iterations == 5):
		fs1_x_offset = np.mean(fs1_x) 
		fs1_y_offset = np.mean(fs1_y)
		fs1_z_offset = np.mean(fs1_z)
		fs2_x_offset = np.mean(fs2_x)
		fs2_y_offset = np.mean(fs2_y)
		fs2_z_offset = np.mean(fs2_z)
	if(iterations>5):
		if "s1" in i:
			fs1 = i[2:-6].replace('&&&','')
			# print fs1
			# print offset
			for j in range(len(fs1)):
				if(j%3 == 0):
					fs1_x.append(10*(ord(fs1[j])-fs1_x_offset))
				elif(j%3 == 1 ):
					fs1_y.append(10*(ord(fs1[j])-fs1_y_offset))
				else:
					fs1_z.append(10*(ord(fs1[j])-fs1_z_offset))
			
		elif "s2" in i:
			fs2 = i[2:-6].replace('&&&','')
			#print fs2
			#print offset
			for j in range(len(fs2)):
				if(j%3 == 0):
					fs2_x.append(10*(ord(fs2[j])-fs2_x_offset))
				elif(j%3 == 1 ):
					fs2_y.append(10*(ord(fs2[j])-fs2_y_offset))
				else:
					fs2_z.append(10*(ord(fs2[j])-fs2_z_offset))



### Low pass filtering


fs1_x_filtered = butter_lowpass_filter(fs1_x, cutoff, fs, order)
fs1_y_filtered = butter_lowpass_filter(fs1_y, cutoff, fs, order)
fs1_z_filtered = butter_lowpass_filter(fs1_z, cutoff, fs, order)

fs2_x_filtered = butter_lowpass_filter(fs2_x, cutoff, fs, order)
fs2_y_filtered = butter_lowpass_filter(fs2_y, cutoff, fs, order)
fs2_z_filtered = butter_lowpass_filter(fs2_z, cutoff, fs, order)



# plt.plot(fs1_x, 'b-', label='data')
# plt.plot(fs1_x_filtered + 1000*np.ones(len(fs1_x_filtered)), 'g-', linewidth=2, label='filtered data')
# plt.legend()
# plt.show()



# plt.plot(fs1_y, 'b-', label='data')
# plt.plot(fs1_y_filtered + 1000*np.ones(len(fs1_y_filtered)), 'g-', linewidth=2, label='filtered data')
# plt.legend()
# plt.show()



# plt.plot(fs1_z, 'b-', label='data')
# plt.plot(fs1_z_filtered + 1000*np.ones(len(fs1_z_filtered)), 'g-', linewidth=2, label='filtered data')
# plt.legend()
# plt.show()



# plt.plot(fs2_x, 'b-', label='data')
# plt.plot(fs2_x_filtered + 1000*np.ones(len(fs2_x_filtered)), 'g-', linewidth=2, label='filtered data')
# plt.legend()
# plt.show()



# plt.plot(fs2_y, 'b-', label='data')
# plt.plot(fs2_y_filtered + 1000*np.ones(len(fs2_y_filtered)), 'g-', linewidth=2, label='filtered data')
# plt.legend()
# plt.show()



# plt.plot(fs2_z, 'b-', label='data')
# plt.plot(fs2_z_filtered + 1000*np.ones(len(fs2_z_filtered)), 'g-', linewidth=2, label='filtered data')
# plt.legend()
# plt.show()

### plotting


plt.plot(fs1_x_filtered)
plt.plot(fs1_y_filtered + 2000*np.ones(len(fs1_y_filtered)))
plt.plot(fs1_z_filtered + 4000*np.ones(len(fs1_z_filtered)))
plt.show()

plt.plot(fs2_x_filtered)
plt.plot(fs2_y_filtered + 2000*np.ones(len(fs2_y_filtered)))
plt.plot(fs2_z_filtered + 4000*np.ones(len(fs2_z_filtered)))
plt.show()

