# practice_1
я построил НКА с однобуквенными переходами
Запустил от каждого узла бфс. Пока есть соседнее ребро, по которому можно пройти по пустому слову либо по нужной букве - шед дальше, и так, пока не попал в подслово x^k. Потом просто по всем парам вершин, между которыми есть такой путь, нашел минимум от начала до этого куска и от куска до конца.
