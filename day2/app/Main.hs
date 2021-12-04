module Main where

import Control.Arrow ((&&&))
import Data.List (foldl')
import System.Environment (getArgs)

data Direction = Up | Down | Forward deriving Show
data Motion = Motion Direction Int deriving Show
data Position = Position {depth, horizontal :: Int} deriving Show

type Input = [Motion]

part1 :: Input -> Int
part1 = ((*) <$> depth <*> horizontal) . foldl' move (Position 0 0)
  where move (Position d x) (Motion m n) = case m of
          Forward -> Position d (x + n)
          Up -> Position (d + n) x
          Down -> Position (d - n) x


data RealPosition = RealPosition { x, d, aim :: Int } deriving Show

part2 :: Input -> Int
part2 = ((*) <$> d <*> x) . foldl' move (RealPosition 0 0 0)
  where move (RealPosition x d aim) (Motion m n) = case m of
          Forward -> RealPosition (x + n) (d + (n * aim)) aim
          Down -> RealPosition x d (aim + n)
          Up -> RealPosition x d (aim - n)

prepare :: String -> Input
prepare = map (parse . words) . lines
  where parse [dir, n] = Motion d (read n)
          where d = case dir of
                      "down" -> Down
                      "up" -> Up
                      "forward" -> Forward

main :: IO ()
main = do
    args <- getArgs
    readFile (head args) >>= print . (part1 &&& part2) . prepare