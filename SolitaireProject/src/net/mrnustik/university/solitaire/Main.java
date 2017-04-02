/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire;

import net.mrnustik.university.solitaire.factory.SolitaireFactory;
import net.mrnustik.university.solitaire.model.Board;

/**
 *
 * @author micha
 */
public class Main {
    public static void main(String args[])
    {
        Board board = new Board(new SolitaireFactory());
    }
}
