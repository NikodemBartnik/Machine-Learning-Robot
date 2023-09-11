import numpy as np
import pandas as pd
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import f_classif
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import matplotlib.pyplot as plt
from sklearn import tree
from sklearn.metrics import accuracy_score, classification_report
from micromlgen import port

def letters_to_colors(letters):
    color_mapping = {
        'F': 'blue',
        'L': 'white',
        'R': 'white',
        'G': 'cyan',
        'I': 'magenta'
    }
    
    return [color_mapping.get(letter, 'black') for letter in letters]

data = pd.read_csv('I:/arduino lidar/visualizer/test4and5.txt', header=None)
data.rename(columns={data.columns[-1]: 'Label'}, inplace=True)
data = data[(data['Label'] != 'L') & (data['Label'] != 'R') & (data['Label'] != 'H')]
data.reset_index(drop=True, inplace=True)
X = data.iloc[:, :-1]
y = data.iloc[:, -1]

label_encoder = LabelEncoder()
y = label_encoder.fit_transform(y)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

k = 20
k_best = SelectKBest(score_func=f_classif, k=k)
k_best.fit(X_train, y_train)

selected_feature_indices = k_best.get_support(indices=True)
print("selected_feature_indices: ", selected_feature_indices)

# plt.scatter(data.iloc[:, selected_feature_indices[0]], data.iloc[:, selected_feature_indices[19]], c = letters_to_colors(data.iloc[:, -1]), s=8)
# plt.show()

clf = tree.DecisionTreeClassifier()
clf.fit(X_train.iloc[:, selected_feature_indices], y_train)

y_pred = clf.predict(X_test.iloc[:, selected_feature_indices])

accuracy = accuracy_score(y_test, y_pred)
print(f'Accuracy: {accuracy}')

class_names = label_encoder.classes_
report = classification_report(y_test, y_pred, target_names=class_names, zero_division=0)
print('Classification Report:\n', report)