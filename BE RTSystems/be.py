import xml.etree.ElementTree as ET

tree = ET.parse('xmlB1-periodique.xml')
root = tree.getroot()

# print root.tag, root.attrib

for child in root:
    print child.tag, child.attrib

# <message>
#     <nom>DYYJ-0</nom>
#     <type>CP</type>
#     <frequence>6.25</frequence>
#     <taille_mes>00001</taille_mes>
#     <emetteur>DYYJ</emetteur>
#     <recepteur>SXJJ</recepteur>
# </message>
