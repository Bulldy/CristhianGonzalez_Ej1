import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

v0=np.genfromtxt("0.txt")
v1=np.genfromtxt("1.txt")
v2=np.genfromtxt("2.txt")
v3=np.genfromtxt("3.txt")

v=np.concatenate((v0,v1,v2,v3))

with PdfPages('placas.pdf') as pdf:
    fig=plt.figure()
    plt.imshow(v,interpolation="bilinear")
    plt.colorbar()
    plt.title('Potencial (V)')
    frame1=plt.gca()
    frame1.axes.get_xaxis().set_visible(False)
    frame1.axes.get_yaxis().set_visible(False)
    pdf.savefig()
    plt.close()
