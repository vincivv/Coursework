# ============================================================
# Solvers — Backtracking (two different implementations)
#           Placeholder for BFS, DFS
# Authors: S. El Alaoui and ChatGPT 5
# ============================================================

import math
from collections import deque
import time

from the3jugs import *

"""
Depth-first backtracking with simple 'explored' pruning.
Stores the best (lowest-cost) path of states encountered to any goal.
This is a recursive implementation. 

returns a dictionary with the following informatin: 
    best_cost= path cost (i.e. number of steps from start to the goal),
    best_path= [s_0, ..., s*],
    found= boolean : path found or not 
    expanded= # of state explored 
        
"""
class BacktrackingSearch:
    def __init__(self, problem: SearchProblem):
        self.best_cost = math.inf
        self.best_path = None
        self.explored = set()
        self.problem = problem

    def recurse(self, state, path, cost: int):
        if self.problem.is_end(state):
       
            if cost < self.best_cost:
                self.best_cost = cost
                self.best_path = path[:]  # copy
                # print(self.best_cost)
            return

        for action in self.problem.actions(state):
            next_state = self.problem.succ(state, action)
            key = str(next_state)
            # key = next_state
            if key not in self.explored:
                
                self.explored.add(key)
                
                self.recurse(next_state, path + [next_state], cost + self.problem.cost(state, action))

    def solve(self):
        start = self.problem.start_state()
        self.explored.add(str(start))
        self.recurse(start, [], 0)
        return dict(
            best_cost=self.best_cost,
            best_path=[self.problem.start_state()] + (self.best_path or []),
            found=(self.best_path is not None),
            expanded=len(self.explored),
        )

"""
Depth-first backtracking with simple 'explored' pruning (iterative).
Stores the best (lowest-cost) path of states encountered to any goal.
This is an iterative implementation. 

returns a dictionary with the following informatin: 
    best_cost= path cost (i.e. number of steps from start to the goal),
    best_path= [s_0, ..., s*],
    found= boolean : path found or not 
    expanded= # of state explored

"""
class BacktrackingSearchIterative:
    def __init__(self, problem):
        self.best_cost = math.inf
        self.best_path = None
        self.explored = set()
        self.problem = problem

    def solve(self):
        start = self.problem.start_state()
        start_key = str(start)
        self.explored.add(start_key)

        # Stack holds tuples: (state, path_from_after_start, cost_so_far)
        stack = [(start, [], 0)]

        while stack:
            state, path, cost = stack.pop()

            # Goal check
            if self.problem.is_end(state):
                if cost < self.best_cost:
                    self.best_cost = cost
                    self.best_path = path[:]  # copy
                continue

            # Expand
            actions = list(self.problem.actions(state))
            # To match recursive DFS order, push in reverse so first action is explored first.
            for action in reversed(actions):
                next_state = self.problem.succ(state, action)
                key = str(next_state)
                if key not in self.explored:
                    self.explored.add(key)
                    next_cost = cost + self.problem.cost(state, action)
                    stack.append((next_state, path + [next_state], next_cost))

        return dict(
            best_cost=self.best_cost,
            best_path=[self.problem.start_state()] + (self.best_path or []),
            found=(self.best_path is not None),
            expanded=len(self.explored),
        )


"""
Add an iterative implementation of DFS.
BFS explores nodes level by leveland is guaranteed to find a goal at minimum depth (the fewest steps).

returns a dictionary with the following informatin: 
    best_cost= path cost (i.e. number of steps from start to the goal),
    best_path= [s_0, ..., s*],
    found= boolean : path found or not 
    expanded= # of state explored
"""
class BFSSearch:
    def __init__(self, problem: SearchProblem):
        self.problem = problem

    def solve(self):
        start_time = time.time()
        start = self.problem.start_state()

        if self.problem.is_end(start):
            return dict(best_cost=0, best_path=[start], found=True, expanded=1,
                        b=0, D=0, d=0, time=time.time() - start_time)

        # Queue: (state, path, cost, depth)
        queue = deque()
        queue.append((start, [], 0, 0))

        explored = set()
        explored.add(str(start))

        total_successors = 0
        total_expansions = 0
        max_depth = 0

        while queue:
            state, path, cost, depth = queue.popleft()
            total_expansions += 1
            max_depth = max(max_depth, depth)

            for action in self.problem.actions(state):
                next_state = self.problem.succ(state, action)
                key = str(next_state)

                if key not in explored:
                    explored.add(key)
                    total_successors += 1
                    next_path = path + [next_state]
                    next_cost = cost + self.problem.cost(state, action)
                    next_depth = depth + 1

                    if self.problem.is_end(next_state):
                        elapsed = time.time() - start_time
                        b = total_successors / total_expansions if total_expansions > 0 else 0
                        return dict(
                            best_cost=next_cost,
                            best_path=[start] + next_path,
                            found=True,
                            expanded=len(explored),
                            b=round(b, 4),
                            D=max(max_depth, next_depth),
                            d=next_depth,
                            time=elapsed,
                        )

                    queue.append((next_state, next_path, next_cost, next_depth))

        elapsed = time.time() - start_time
        b = total_successors / total_expansions if total_expansions > 0 else 0
        return dict(
            best_cost=math.inf,
            best_path=[start],
            found=False,
            expanded=len(explored),
            b=round(b, 4),
            D=max_depth,
            d=None,
            time=elapsed,
        )

"""
Add an iterative implementation of DFS.
DFS explores along a path as deep as possible before backtracking 
and returns the first solution found, which may not be the shortest.

returns a dictionary with the following informatin: 
    best_cost= path cost (i.e. number of steps from start to the goal),
    best_path= [s_0, ..., s*],
    found= boolean : path found or not 
    expanded= # of state explored
"""
class DFSSearch:


    def __init__(self, problem: SearchProblem):
        self.problem = problem

    def solve(self):
        start_time = time.time()
        start = self.problem.start_state()

        if self.problem.is_end(start):
            return dict(best_cost=0, best_path=[start], found=True, expanded=1,
                        b=0, D=0, d=0, time=time.time() - start_time)

        # Stack: (state, path, cost, depth)
        stack = [(start, [], 0, 0)]

        explored = set()
        explored.add(str(start))

        total_successors = 0
        total_expansions = 0
        max_depth = 0

        while stack:
            state, path, cost, depth = stack.pop()
            total_expansions += 1
            max_depth = max(max_depth, depth)

            if self.problem.is_end(state):
                elapsed = time.time() - start_time
                b = total_successors / total_expansions if total_expansions > 0 else 0
                return dict(
                    best_cost=cost,
                    best_path=[start] + path,
                    found=True,
                    expanded=len(explored),
                    b=round(b, 4),
                    D=max_depth,
                    d=depth,
                    time=elapsed,
                )

            for action in reversed(self.problem.actions(state)):
                next_state = self.problem.succ(state, action)
                key = str(next_state)
                if key not in explored:
                    explored.add(key)
                    total_successors += 1
                    next_cost = cost + self.problem.cost(state, action)
                    stack.append((next_state, path + [next_state], next_cost, depth + 1))

        elapsed = time.time() - start_time
        b = total_successors / total_expansions if total_expansions > 0 else 0
        return dict(
            best_cost=math.inf,
            best_path=[start],
            found=False,
            expanded=len(explored),
            b=round(b, 4),
            D=max_depth,
            d=None,
            time=elapsed,
        )






