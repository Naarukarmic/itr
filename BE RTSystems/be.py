import xml.etree.ElementTree as ET
import math

tree = ET.parse('xmlB1-periodique.xml')
root = tree.getroot()

elt = {} # Uniques dict
mes = {} # Messages dict
master = 'SXJJ'

for message in root:
    tmp = {}
    name = ''
    for leaf in message:
        # Building uniques dict
        if elt.get(leaf.tag) is None:
            elt[leaf.tag] = []
        if leaf.text not in elt[leaf.tag]:
            elt[leaf.tag].append(leaf.text)

        # Building messages dict
        if leaf.tag == 'nom':
            name = leaf.text
        else:
            tmp[leaf.tag] = leaf.text
    mes[name] = tmp

# Total messages size
for k,v in mes.iteritems():
    mes_size = 0
    if v['emetteur'] == master or v['recepteur'] == master:
        mes_size += 56
    else:
        mes_size += 108
    mes_size += int(v['taille_mes']) * 20
    v['taille_tot'] = mes_size

# Individual transmission delay
speed = 1000000.0 # bps -> delay in s
# speed = 1 # Mbps -> delay in us
for k,v in mes.iteritems():
    mes_size = v['taille_tot']
    delay = mes_size / speed
    v['trans_delay'] = delay

# Transaction table
freq = map(float, elt['frequence'])
period = map(lambda x: 1/x, freq)
micro = min(period)
macro = max(period) # Technically LCM

# Necessary condition for scheduling
cond = 0
for k,v in mes.iteritems():
    cond += v['trans_delay']
cond /= micro
print 'CNS: ', cond, '<= 1'

# WCRT
# Divide messages per priority
p = {'highest': {},
     'high': {},
     'low': {},
     'lowest': {}}
for k,v in mes.iteritems():
    prio = float(v['frequence'])
    if prio == 50: 
        p['highest'][k] = [0, 0]
    elif prio == 25:
        p['high'][k] = [0, 0]
    elif prio == 12.5:
        p['low'][k] = [0, 0]
    elif prio == 6.25:
        p['lowest'][k] = [0, 0]

# Then recursively calculate the WCRT
# Max delay in a priority (or multiple) priority dict
def max_delay(d, d1=None, d2=None):
    res = 0
    for k,v in d.iteritems():
        if mes[k]['trans_delay'] > res:
            res = mes[k]['trans_delay']
    if d1 is not None:
        res = max(res, max_delay(d1))
    if d2 is not None:
        res = max(res, max_delay(d2))
    return res

# Sum of all delays in one (or multiple) priority dict, avoiding k
def sum_dict(k, d, d1=None, d2=None, d3=None):
    res = 0
    for k2,v in d.iteritems():
        if k2 != k:
            res_i = mes[k2]['trans_delay']
            res_i *= math.ceil(v[0]*float(mes[k2]['frequence']))
            res += res_i
    if d1 is not None:
        res += sum_dict(k, d1, d2, d3)
    return res

# Apply the formula to all level of priorities
for i in range(100):
    for k,v in p['highest'].iteritems():
        v[1] = mes[k]['trans_delay']
        v[1] += max_delay(p['high'], p['low'], p['lowest'])
        v[1] += sum_dict(k, p['highest'])
    for k,v in p['high'].iteritems():
        v[1] = mes[k]['trans_delay']
        v[1] += max_delay(p['low'], p['lowest'])
        v[1] += sum_dict(k, p['highest'], p['high'])
    for k,v in p['low'].iteritems():
        v[1] = mes[k]['trans_delay']
        v[1] += max_delay(p['lowest'])
        v[1] += sum_dict(k, p['highest'], p['high'], p['low'])
    for k,v in p['lowest'].iteritems():
        v[1] = mes[k]['trans_delay']
        v[1] += sum_dict(k, p['highest'], p['high'], p['low'], p['lowest'])

    # Swap old value for new
    p = {l: {k: [v[1], v[1]] for k,v in u.iteritems()} 
            for l,u in p.iteritems()}

# Concatenate all priority dicts and simplify the values
concat = {}
for k,v in p.iteritems():
    concat = dict(concat, **v)
final = {k: v[0] for (k,v) in concat.iteritems()}

# Add the WCRT to the mes dict
for k,v in final.iteritems():
    mes[k]['wcrt'] = v

# Calculate d2
for k,v in mes.iteritems():
    v['d2'] =  v['wcrt'] - v['trans_delay']

# Prepare report
report = {k: {l: str(u) for l,u in v.iteritems()} for k,v in mes.iteritems()}

for k,v in report.iteritems():
    print k,v

# <message>
#     <nom>DYYJ-0</nom>
#     <type>CP</type>
#     <frequence>6.25</frequence>
#     <taille_mes>00001</taille_mes>
#     <emetteur>DYYJ</emetteur>
#     <recepteur>SXJJ</recepteur>
# </message>
