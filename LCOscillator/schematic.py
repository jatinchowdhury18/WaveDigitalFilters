import SchemDraw
from SchemDraw import dsp
import SchemDraw.elements as e
import matplotlib.pyplot as plt
plt.style.use('dark_background')

d = SchemDraw.Drawing(fontsize=12, color='white')
d.add(e.DOT, label='Input')

d.add(e.LINE, l=0.5)
d.add(e.SWITCH_SPST, l=1)

I1 = d.add(e.INDUCTOR, d='right', label='I1')
C1 = d.add(e.CAP, d='down', label='C1')
d.add(e.GND)

d.add(e.LINE, d='right', l=1, xy = I1.end)
d.add(e.DOT, label='Output')
d.draw()
