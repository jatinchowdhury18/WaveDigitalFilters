import SchemDraw
from SchemDraw import dsp
import SchemDraw.elements as e
import matplotlib.pyplot as plt
plt.style.use('dark_background')

d = SchemDraw.Drawing(fontsize=12, color='white')
d.add(e.DOT, label='Input')

R1 = d.add(e.RES, d='right', label='R1')
C1 = d.add(e.CAP, d='down', label='C1')
d.add(e.GND)

L1 = d.add(e.LINE, d='right', l=2, xy=R1.end)
D1 = d.add(e.DIODE, d='down')
G = d.add(e.GND)

L2 = d.add(e.LINE, d='right', l=2, xy=L1.end)
D2 = d.add(e.DIODE, d='down', reverse=True)
d.add(e.GND)

d.add(e.LINE, d='right', l=1, xy = L2.end)
d.add(e.DOT, label='Output')
d.draw()
