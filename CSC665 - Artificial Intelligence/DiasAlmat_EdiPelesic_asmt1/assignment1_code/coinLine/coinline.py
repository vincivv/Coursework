# coinline.py

class State:
    def __init__(self, coins, pScore=0, aiScore=0, turn='player'): 
        self.coins = coins
        self.pScore = pScore
        self.aiScore = aiScore
        self.turn = turn


def player(state):
    """
    Returns whose turn it is.
    """
    return state.turn


def actions(state):
    """
    Returns list of valid actions.
    Each action is ('L' or 'R', 1 or 2)
    """
    acts = []
    n = len(state.coins)

    if n >= 1:
        acts.append(('L', 1))
        acts.append(('R', 1))
    if n >= 2:
        acts.append(('L', 2))
        acts.append(('R', 2))

    return acts


def succ(state, action):
    """
    Returns NEW state after applying action.
    Does NOT modify original state.
    """
    if action not in actions(state):
        raise Exception("Invalid action")

    side, num = action

    # Copy values (do NOT mutate original)
    new_coins = state.coins.copy()
    new_p = state.pScore
    new_ai = state.aiScore

    # Take coins
    if side == 'L':
        taken = new_coins[:num]
        new_coins = new_coins[num:]
    else:  # 'R'
        taken = new_coins[-num:]
        new_coins = new_coins[:-num]

    value = sum(taken)

    # Update score
    if state.turn == 'player':
        new_p += value
        next_turn = 'ai'
    else:
        new_ai += value
        next_turn = 'player'

    return State(new_coins, new_p, new_ai, next_turn)


def terminal(state):
    """
    Game ends when no coins remain.
    """
    return len(state.coins) == 0


def utility(state):
    """
    Returns scores tuple.
    """
    return (state.pScore, state.aiScore)


def winner(state):
    """
    Returns 'player', 'ai', or None.
    """
    if not terminal(state):
        return None

    if state.pScore > state.aiScore:
        return 'player'
    elif state.aiScore > state.pScore:
        return 'ai'
    else:
        return None


# --- Helper evaluation for minimax ---
def evaluate(state):
    """
    AI is maximizing player.
    Higher value is better for AI.
    """
    return state.aiScore - state.pScore


def minimax(state, is_maximizing):
    """
    Returns (best_value, best_action)
    """
    if terminal(state):
        return evaluate(state), None

    possible_actions = actions(state)

    if is_maximizing:
        best_val = float("-inf")
        best_action = None

        for action in possible_actions:
            new_state = succ(state, action)
            val, _ = minimax(new_state, False)

            if val > best_val:
                best_val = val
                best_action = action

        return best_val, best_action

    else:
        best_val = float("inf")
        best_action = None

        for action in possible_actions:
            new_state = succ(state, action)
            val, _ = minimax(new_state, True)

            if val < best_val:
                best_val = val
                best_action = action

        return best_val, best_action  