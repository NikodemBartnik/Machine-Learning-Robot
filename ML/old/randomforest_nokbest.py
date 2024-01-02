import numpy as np
import pandas as pd
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import f_classif
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import matplotlib.pyplot as plt
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, classification_report
from micromlgen import port


data = pd.read_csv('I:/ml racing robots/data/racing1.txt', header=None)
data.rename(columns={data.columns[-1]: 'Label'}, inplace=True)
data = data[(data['Label'] != 'L') & (data['Label'] != 'R') & (data['Label'] != 'H')]
data.reset_index(drop=True, inplace=True)
X = data.iloc[:, :-1]
y = data.iloc[:, -1]

label_encoder = LabelEncoder()
y = label_encoder.fit_transform(y)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

clf = RandomForestClassifier(max_depth=3, random_state=42)
clf.fit(X_train, y_train)

y_pred = clf.predict(X_test)

accuracy = accuracy_score(y_test, y_pred)
print(f'Accuracy: {accuracy}')

class_names = label_encoder.classes_
report = classification_report(y_test, y_pred, target_names=class_names, zero_division=0)
print('Classification Report:\n', report)

# arduino_code = open("arduino_random_forest_racing1.c", mode="w+")
# arduino_code.write(port(clf))