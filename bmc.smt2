(declare-fun v1 () Int)
(declare-fun v2 () Int)
(declare-fun v3 () Int)
(declare-fun v4 () Int)
(declare-fun v5 () Int)
(declare-fun v6 () Int)
(declare-fun v10 () Int)
(declare-fun v7 () Int)
(declare-fun v8 () Int)
(declare-fun v9 () Int)
(declare-fun v11 () Int)
(declare-fun v12 () Bool)

(assert (= v1 10))
(assert (= v2 3))
(assert (= v3 (div v1 v2)))
(assert (= v4 5))
(assert (= v5 5))
(assert (= v6 (- v4 v5)))
(assert (= v10 x))
(assert (= v7 x))
(assert (= v8 2))
(assert (= v9 (mod v7 v8)))
(assert (= v11 v9)
(assert (= v12 (< v3 v11)))