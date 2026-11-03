struct ExpressionParser {
    static constexpr char UNARY_NEG = '~';
    int priority(char op) const {
        // Menos unario (-x), asociativo a derecha
        if (op == UNARY_NEG) return 3;
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return -1;
    }
    void apply_op(stack<ll>& st, char op) const {
        if (op == UNARY_NEG) {
            ll x = st.top();
            st.pop();
            st.push(-x);
        } else {
            ll b = st.top();
            st.pop();
            ll a = st.top();
            st.pop();
            if (op == '+') st.push(a + b);
            if (op == '-') st.push(a - b);
            if (op == '*') st.push(a * b);
            if (op == '/') st.push(a / b);
        }
    }
    ll evaluate(const string& s) const {
        stack<ll> st;
        stack<char> op;
        bool may_be_unary = true;
        for (int i = 0; i < sz(s); ++i) {
            if (isspace((unsigned char)s[i])) continue;
            if (s[i] == '(') {
                op.push('(');
                may_be_unary = true;
            } else if (s[i] == ')') {
                while (op.top() != '(') {
                    apply_op(st, op.top());
                    op.pop();
                }
                op.pop();
                may_be_unary = false;
            } else if (s[i] == '+' || s[i] == '-' ||
                       s[i] == '*' || s[i] == '/') {
                char cur_op = s[i];
                if (may_be_unary &&
                    (cur_op == '+' || cur_op == '-')) {
                    if (cur_op == '-') cur_op = UNARY_NEG;
                    else continue;
                }
                while (!op.empty() &&
                       ((cur_op != UNARY_NEG &&
                         priority(op.top()) >=
                             priority(cur_op)) ||
                        (cur_op == UNARY_NEG &&
                         priority(op.top()) >
                             priority(cur_op)))) {
                    apply_op(st, op.top());
                    op.pop();
                }
                op.push(cur_op);
                may_be_unary = true;
            } else {
                ll val = 0;
                while (i < sz(s) && isdigit(s[i]))
                    val = val * 10 + (s[i++] - '0');
                --i;
                st.push(val);
                may_be_unary = false;
            }
        }
        while (!op.empty()) {
            apply_op(st, op.top());
            op.pop();
        }
        return st.top();
    }
};
