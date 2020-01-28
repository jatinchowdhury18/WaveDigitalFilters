import SchemDraw
from SchemDraw import dsp
import SchemDraw.elements as e
import matplotlib.pyplot as plt
plt.style.use('dark_background')

d = SchemDraw.Drawing(fontsize=12, color='white')
d.add(e.DOT, label='Input')

R1 = d.add(e.RES, d='right', label='R1')
R2 = d.add(e.RES, d='down', label='R2')
d.add(e.GND)

d.add(e.LINE, d='right', l=1, xy = R1.end)
d.add(e.DOT, label='Output')
d.draw()
