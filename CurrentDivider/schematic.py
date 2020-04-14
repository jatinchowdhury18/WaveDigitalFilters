#%%
import SchemDraw
from SchemDraw import dsp
import SchemDraw.elements as e
import matplotlib.pyplot as plt
plt.style.use('dark_background')

#%%
d = SchemDraw.Drawing(fontsize=12, color='white')
d.add(e.SOURCE_I, label='input', d='down')
d.add(e.LINE, d='right')
R1 = d.add(e.RES, d='up', label='R1')
d.add(e.LINE, d='left')
d.labelI(R1, 'output', top=False, reverse=True)

d.add(e.LINE, d='right', xy=R1.start)
R2 = d.add(e.RES, d='up', label='R2')
d.add(e.LINE, d='left')

d.draw()

