import numpy as np

before_opt = [
    1085028165,
    1093385365,
    1156352390,
    1047397737,
    1039771778,
    1044891117,
    1036473995,
    1054563483
]

after_opt = [
    371836623,
    513393055,
    361827946,
    504020401,
    507487802,
    358567594,
    523178863,
    508857641
]

mean_before = np.mean(before_opt)
mean_after = np.mean(after_opt)
speedup_factor = mean_before / mean_after
speedup_percent = (1 - mean_after / mean_before) * 100

print(mean_before / (10 ** 9), mean_after / (10 ** 9), speedup_factor, speedup_percent)
