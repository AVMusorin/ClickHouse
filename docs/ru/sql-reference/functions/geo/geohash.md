---
slug: /ru/sql-reference/functions/geo/geohash
sidebar_label: "Функции для работы с системой Geohash"
---

# Функции для работы с системой Geohash {#geohash}

[Geohash](https://en.wikipedia.org/wiki/Geohash) — это система геокодирования, которая делит поверхность Земли на участки в виде "решетки", и каждую ячейку решетки кодирует в виде строки из букв и цифр. Система поддерживает иерархию (вложенность) ячеек, поэтому чем точнее определена геопозиция, тем длиннее строка с кодом соответствующей ячейки.

Для ручного преобразования географических координат в строку geohash можно использовать сайт [geohash.org](http://geohash.org/).

## geohashEncode {#geohashencode}

Кодирует широту и долготу в строку [geohash](/ru/sql-reference/functions/geo/geohash).

``` sql
geohashEncode(longitude, latitude, [precision])
```

**Входные значения**

-   longitude — долгота. Диапазон — `[-180°, 180°].`
-   latitude — широта. Диапазон — `[-90°, 90°].`
-   precision — длина результирующей строки, по умолчанию `12`. Опционально. Целое число в диапазоне `[1, 12]`. Любое значение меньше, чем `1` или больше `12` автоматически преобразуются в `12`.

**Возвращаемые значения**

-   Строка с координатой, закодированной модифицированной версией алфавита base32.

**Пример**

``` sql
SELECT geohashEncode(-5.60302734375, 42.593994140625, 0) AS res;
```

``` text
┌─res──────────┐
│ ezs42d000000 │
└──────────────┘
```

## geohashDecode {#geohashdecode}

Декодирует любую строку, закодированную в [geohash](/ru/sql-reference/functions/geo/geohash), на долготу и широту.

``` sql
geohashDecode(geohash_string)
```

**Входные значения**

-   `geohash_string` — строка, содержащая geohash.

**Возвращаемые значения**

-   `(longitude, latitude)` — широта и долгота. Кортеж из двух значений типа `Float64`.

**Пример**

``` sql
SELECT geohashDecode('ezs42') AS res;
```

``` text
┌─res─────────────────────────────┐
│ (-5.60302734375,42.60498046875) │
└─────────────────────────────────┘
```

## geohashesInBox {#geohashesinbox}

Формирует массив участков, которые находятся внутри или пересекают границу заданного участка на поверхности. Каждый участок описывается строкой [geohash](/ru/sql-reference/functions/geo/geohash) заданной точности.

**Синтаксис**

``` sql
geohashesInBox(longitude_min, latitude_min, longitude_max, latitude_max, precision)
```

**Аргументы**

-   `longitude_min` — минимальная долгота. Диапазон возможных значений: `[-180°, 180°]`. Тип данных: [Float](../../../sql-reference/data-types/float.md)).
-   `latitude_min` — минимальная широта. Диапазон возможных значений: `[-90°, 90°]`. Тип данных: [Float](../../../sql-reference/data-types/float.md).
-   `longitude_max` — максимальная долгота. Диапазон возможных значений: `[-180°, 180°]`. Тип данных: [Float](../../../sql-reference/data-types/float.md).
-   `latitude_max` — максимальная широта. Диапазон возможных значений: `[-90°, 90°]`. Тип данных: [Float](../../../sql-reference/data-types/float.md).
-   `precision` — точность geohash. Диапазон возможных значений: `[1, 12]`. Тип данных: [UInt8](../../../sql-reference/data-types/int-uint.md).

:::info Замечание
Все передаваемые координаты должны быть одного и того же типа: либо `Float32`, либо `Float64`.
:::

**Возвращаемые значения**

-   Массив строк, описывающих участки, покрывающие заданный участок. Длина каждой строки соответствует точности geohash. Порядок строк — произвольный.
-   \[\] - Если переданные минимальные значения широты и долготы больше соответствующих максимальных значений, функция возвращает пустой массив.

Тип данных: [Array](../../../sql-reference/data-types/array.md)([String](../../../sql-reference/data-types/string.md)).

:::info Замечание
Если возвращаемый массив содержит свыше 10 000 000 элементов, функция сгенерирует исключение.
:::

**Пример**

Запрос:

``` sql
SELECT geohashesInBox(24.48, 40.56, 24.785, 40.81, 4) AS thasos;
```

Результат:

``` text
┌─thasos──────────────────────────────────────┐
│ ['sx1q','sx1r','sx32','sx1w','sx1x','sx38'] │
└─────────────────────────────────────────────┘
```
