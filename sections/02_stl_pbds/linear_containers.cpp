void linear_containers_demo() {
    // 1. std::deque (Doble cola + acceso aleatorio O(1))
    deque<int> dq;
    dq.push_back(20);    // [20]
    dq.push_front(10);   // [10, 20]
    int f = dq.front();  // 10
    int b = dq.back();   // 20
    int x = dq[1];       // 20 (acceso por indice O(1))
    dq.pop_front();      // Quita el 10
    dq.pop_back();       // Quita el 20

    // 2. std::queue (FIFO - Primero en entrar, primero en
    // salir)
    queue<int> q;
    q.push(1);
    q.push(2);
    int primero = q.front();  // 1
    q.pop();                  // Elimina el frente (1)

    // 3. std::stack (LIFO - Ultimo en entrar, primero en
    // salir)
    stack<int> st;
    st.push(10);
    st.push(20);
    int tope = st.top();  // 20
    st.pop();             // Elimina el tope (20)
}
