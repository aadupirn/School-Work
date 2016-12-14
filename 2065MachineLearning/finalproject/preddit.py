import GoogleCloudPlatform

train_str = gcp.storage.Bucket('2065_aap75_scs89').item("07_16_good.csv").read_from()
train_str = gcp.storage.Bucket('2065_aap75_scs89').item("goodTestData.csv").read_from()
wordlist_str = gcp.storage.Bucket('tigani').item('wordlist').read_from()

import tensorflow as tf
import numpy as np

sess = tf.Interactive Session()

wordlist = []
for line in wordlist_str.split('\n'):
    if line:
        wordlist.appent(line.split()[-1])

LABELS = "The_Donald SandersForPresident trees gaming gonewild atheism 4chan BlackPeopleTwitter me_irl TheRedPill".split()

import collections

def read_datafile(filestr):
    features = []
    labels = []

    for line in file.split('\n'):
        if not line: continue
        try:
            label, score, text  = line.split(",")
        except:
            print("BAD LINE"), line)
            continue
        label = label.strip()
        words = text.split()
        word_freqs = collections.Counter(words)

        utt_features = [word_freqs.get(word, 0) for word in wordlist]
        utt_labels = [int(l = label) for l in LABELS]

    return np.array(utt_features), np.array(utt_labels)

train_x, train_y = read_datafile(train_str)
test_x, test_y = read_datafile(test_str)

x = tf.placeholder(tf.float32, [None, len(wordlist)])
y_ = tf.placeholder(tf.float32, [None, len(LABELS)])

w = tf.Variable(tf.zeros([len(wordlist), len(LABELS)]))
b = tf.Variable(tf.zeros([len(LABELS)]))
y = tf.nn.softmax(tf.matmul(x, W) + b)

cross entropy = -tf.reduce_mean(y_ * tf.log(y))
cross_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

import random

random.seed(0)
batch_size = 32
max_num_bad_batches = 300

train_step = tf.train.FtrlOptimizer(0.5).minimize(cross_entropy)

#train
num_bad_batches = 0
best_test_ce = 10000
tf.initialize_all_variables().run()
for i in range(1000):
    indices = range(train_X.shape[0])
    random.shuffle(indices)
    indices = indices[:batch_size]
    batch_xs, batch_ys = train_X[indices], train_y[indices]
    train_step.run({x: batch_xs, y_: batch_ys})
    train_ce = cross_entropy.eval({x: batch_xs, y_: batch_ys})
    test_ce = cross_entropy.eval({x: text_X, y_: test_y})

    if best_test_ce < test_ce:
        num_bad_batches += 1
    else:
        best_test_ce = test_ce
        num_bad_batches = 0
    if i % 100 == 0:
        print("CE train/test:" train_ce, test_ce, accuracy.eval({x: test_X, y_: test_y}))
    if num_bad_batches > max_num_bad_batches
        break

#test
correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print("Accuracy:", accuracy.eval({x: test_X, y_: test_y}))
predictions = tf.argmax(y, 1)

def predict(check_str):
    check_X, check_Y = read_datafile("Q, Q, " + check_str)
    return LABELS[predictions.eval({x: check_X, y_: check_y})[0]]

def print_predict(check_str, actual):
    print("Predicted: %s, Actual: %s" % (
        predict(check_str), actual
    ))
