###### --------------------------------------------
## The author of these scripts is T. D. Devlin 
###### --------------------------------------------

from logic import *

AKnight = Symbol("A is a Knight")
AKnave = Symbol("A is a Knave")

BKnight = Symbol("B is a Knight")
BKnave = Symbol("B is a Knave")

CKnight = Symbol("C is a Knight")
CKnave = Symbol("C is a Knave")

# Puzzle 1
# A says "I am both a knight and a knave."
# ----------------------------------------
##   write the statement(s) in PL
stat = And(AKnight, AKnave)  # "I am both a knight and a knave."
##   Fill in the knowledge base
knowledge1 = And(
    # Structure: A is exactly one of knight or knave
    Or(AKnight, AKnave),
    Not(And(AKnight, AKnave)),
    # A's statement: "I am both a knight and a knave."
    # If A is a knight, the statement must be true
    Implication(AKnight, And(AKnight, AKnave)),
    # If A is a knave, the statement must be false
    Implication(AKnave, Not(And(AKnight, AKnave))),
)
# ----------------------------------------

# Puzzle 2
# A says "We are the same kind."
# B says "We are of different kinds."
# ----------------------------------------
##   write the statement(s) in PL
# A's statement: "We are the same kind."
statA = Biconditional(AKnight, BKnight)
# B's statement: "We are of different kinds."
statB = Not(Biconditional(AKnight, BKnight))
stat = And(statA, statB)
##   Fill in the knowledge base
knowledge2 = And(
    # Structure: each is exactly one of knight or knave
    Or(AKnight, AKnave),
    Not(And(AKnight, AKnave)),
    Or(BKnight, BKnave),
    Not(And(BKnight, BKnave)),
    # A says "We are the same kind."
    # same kind means biconditional: AKnight <=> BKnight
    Implication(AKnight, Biconditional(AKnight, BKnight)),
    Implication(AKnave, Not(Biconditional(AKnight, BKnight))),
    # B says "We are of different kinds."
    # different kinds: exactly one is a knight
    Implication(BKnight, Not(Biconditional(AKnight, BKnight))),
    Implication(BKnave, Biconditional(AKnight, BKnight)),
)
# ----------------------------------------

# Puzzle 3
# A says either "I am a knight." or "I am a knave.", but you don't know which.
# B says "A said 'I am a knave'."
# B says "C is a knave."
# C says "A is a knight."
# ----------------------------------------
##   write the statement(s) in PL
# A says one of these (unknown which):
statA_knight = AKnight   # "I am a knight."
statA_knave  = AKnave    # "I am a knave."
# B's two statements:
statB1 = AKnave          # "A said 'I am a knave'."
statB2 = CKnave          # "C is a knave."
# C's statement:
statC = AKnight          # "A is a knight."
stat = And(statB1, statB2, statC)
##   Fill in the knowledge base
knowledge3 = And(
    # Structure: each is exactly one of knight or knave
    Or(AKnight, AKnave),
    Not(And(AKnight, AKnave)),
    Or(BKnight, BKnave),
    Not(And(BKnight, BKnave)),
    Or(CKnight, CKnave),
    Not(And(CKnight, CKnave)),
    # B says "A said 'I am a knave'."
    # If B is a knight, A said "I am a knave":
    #   - A knight saying "I am a knave" => AKnave true => contradicts AKnight
    #   - A knave saying "I am a knave" => knave lies => AKnave false => contradiction
    # So B being a knight always leads to contradiction => B must be a knave
    Implication(BKnight, And(AKnave, Not(AKnave))),
    Implication(BKnave, AKnight),
    # B says "C is a knave."
    Implication(BKnight, CKnave),
    Implication(BKnave, Not(CKnave)),
    # C says "A is a knight."
    Implication(CKnight, AKnight),
    Implication(CKnave, Not(AKnight)),
)
# ----------------------------------------


def main():
    symbols = [AKnight, AKnave, BKnight, BKnave, CKnight, CKnave]
    puzzles = [
        ("Puzzle 1", knowledge1),
        ("Puzzle 2", knowledge2),
        ("Puzzle 3", knowledge3)
    ]
    for puzzle, knowledge in puzzles:
        print(puzzle)
        if len(knowledge.conjuncts) == 0:
            print("    Not yet implemented.")
        else:
            for symbol in symbols:
                if model_check(knowledge, symbol):
                    print(f"    {symbol}")


if __name__ == "__main__":
    main()