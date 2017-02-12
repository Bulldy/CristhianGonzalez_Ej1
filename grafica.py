import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

v0=np.transpose(np.genfromtxt("v0.txt"))
v1=np.transpose(np.genfromtxt("v1.txt"))
v2=np.transpose(np.genfromtxt("v2.txt"))
v3=np.transpose(np.genfromtxt("v3.txt"))
v=np.concatenate((v0,v1,v2,v3))

ex0=np.transpose(np.genfromtxt("ex0.txt"))
ex1=np.transpose(np.genfromtxt("ex1.txt"))
ex2=np.transpose(np.genfromtxt("ex2.txt"))
ex3=np.transpose(np.genfromtxt("ex3.txt"))
ex=np.concatenate((ex0,ex1,ex2,ex3))

ey0=np.transpose(np.genfromtxt("ey0.txt"))
ey1=np.transpose(np.genfromtxt("ey1.txt"))
ey2=np.transpose(np.genfromtxt("ey2.txt"))
ey3=np.transpose(np.genfromtxt("ey3.txt"))
ey=np.concatenate((ey0,ey1,ey2,ey3))

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

    fig=plt.figure()
    y,x=np.mgrid[-len(ex):0,0:len(ey)]
    frame1=plt.gca()
    e1=np.sqrt(ex*ex+ey*ey)
    e=5*e1/e1.max()
    frame1.axes.get_xaxis().set_visible(False)
    frame1.axes.get_yaxis().set_visible(False)
    st=plt.streamplot(x,y,-1*ey,-1*ex,density=2,linewidth=e,color=e1)
    plt.colorbar(st.lines)
    plt.title('Lineas de Campo')
    pdf.savefig()
    plt.close()
