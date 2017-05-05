/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire;

import net.mrnustik.university.solitaire.view.frames.MainFrame;

/**
 * Main class of the application. Used as entry point of the application.
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
class Main {

    /**
     * Entry point method of the applications.
     * Just starts the view and sets it to visible.
     * @param args arguments from command line
     */
    public static void main(String args[]) {
        
        MainFrame frame = new MainFrame();
        frame.setVisible(true);
        
    }
}
