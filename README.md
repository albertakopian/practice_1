# practice_1
я построил нка с однобуквенными переходами. Далее запустил бфс от каждого узла. Пока есть соседнее ребро, по которому можно пройти по пустому слову либо по нужной букве - шел дальше, и так, пока не попал в подслово x^k или пока очередь не стала пустой. Потом просто по всем парам вершин, между которыми есть такой путь, нашел минимум от начала до этого куска и от куска до конца.
