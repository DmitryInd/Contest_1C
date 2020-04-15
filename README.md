# Автор: Инденбом Дмитрий, группа Б05-814
Решение задачи для отбора на кафедру 1С.

Программа строит два дерева: введённых номеров и имён, которые пользователь вносит вначале программы.
Ввод базы производится через строчки: "номер - имя" и заканчивается строчкой "*".
Дальше пользователь выбирает, что делать:
1. Ищет телефон по номеру (неполному, целому или с пропусками)
2. Ищет телефон по имени (неполному или целому)
3. Выходит из программы

Программа ищет номер, "шагая" по узлам дерева, которые представляют из себя цифры/буквы номера/имени.
Дойдя до конца она возвращает все целые телефоны, которые есть в оставшихся разветвлениях дерева.
