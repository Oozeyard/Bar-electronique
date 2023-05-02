import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;
import java.util.Scanner;
import java.io.*;
import java.net.*;

public class Commande extends UnicastRemoteObject {
    Commande() throws RemoteException{
    }
    final static int port = 1909;
    final static int taille = 1024;
    static byte[] buffer = new byte[taille];
    static byte[] bufferIn = new byte[taille];
    static boolean running = true;
    public static void main(String[] args) throws IOException{
        Scanner scanner = new Scanner(System.in);

        try{
            DatagramSocket socket = new DatagramSocket(port);//creation socket controle->commande
            DatagramPacket message = new DatagramPacket(bufferIn, buffer.length);//creation packet reçu
            System.out.println("Démarrage boucle");
            String greeting = """

                                 /$$$$$$$ /$$$$$$/$$$$$$$$/$$   /$$/$$    /$$/$$$$$$$$/$$   /$$/$$   /$$        /$$$$$$ /$$   /$$       /$$$$$$$  /$$$$$$ /$$$$$$$\s
                                | $$__  $|_  $$_| $$_____| $$$ | $| $$   | $| $$_____| $$$ | $| $$  | $$       /$$__  $| $$  | $$      | $$__  $$/$$__  $| $$__  $$
                                | $$  \\ $$ | $$ | $$     | $$$$| $| $$   | $| $$     | $$$$| $| $$  | $$      | $$  \\ $| $$  | $$      | $$  \\ $| $$  \\ $| $$  \\ $$
                                | $$$$$$$  | $$ | $$$$$  | $$ $$ $|  $$ / $$| $$$$$  | $$ $$ $| $$  | $$      | $$$$$$$| $$  | $$      | $$$$$$$| $$$$$$$| $$$$$$$/
                                | $$__  $$ | $$ | $$__/  | $$  $$$$\\  $$ $$/| $$__/  | $$  $$$| $$  | $$      | $$__  $| $$  | $$      | $$__  $| $$__  $| $$__  $$
                                | $$  \\ $$ | $$ | $$     | $$\\  $$$ \\  $$$/ | $$     | $$\\  $$| $$  | $$      | $$  | $| $$  | $$      | $$  \\ $| $$  | $| $$  \\ $$
                                | $$$$$$$//$$$$$| $$$$$$$| $$ \\  $$  \\  $/  | $$$$$$$| $$ \\  $|  $$$$$$/      | $$  | $|  $$$$$$/      | $$$$$$$| $$  | $| $$  | $$
                                |_______/|______|________|__/  \\__/   \\_/   |________|__/  \\__/\\______/       |__/  |__/\\______/       |_______/|__/  |__|__/  |__/
                                                                                                                                                                  \s
                                                                                                                                                                  \s
                                                                                                                                                                  \s
                                                                                                                                                                                               \s
                    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
                    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
                                                                            ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣶⣾⡇⢸⣷⣶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
                                                                            ⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⡟⠛⠛⢻⡇⢸⡟⠛⠛⢻⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀
                                                                            ⠀⠀⠀⠀⢀⣀⣤⣶⡟⠛⠛⠛⠛⢻⡇⢸⡟⠛⠛⠛⠛⢻⣶⣤⣀⡀⠀⠀⠀⠀
                                                                            ⠀⠀⢸⣟⣛⣛⣛⣛⣛⣛⣛⣛⣛⣻⡇⢸⣟⣛⣛⣛⣛⣛⣛⣛⣛⣛⣻⡇⠀⠀
                                                                            ⠀⠀⢸⣟⣉⣉⣉⣉⣉⣉⣉⣉⣉⣹⡇⢸⣏⣉⣉⣉⣉⣉⣉⣉⣉⣉⣻⡇⠀⠀
                                                                            ⠀⢀⡈⢹⣉⣉⣉⣉⣉⣉⣉⣉⣉⣹⡇⢸⣏⣉⣉⣉⣉⣉⣉⣉⣉⣉⡏⢁⡀⠀
                                                                            ⠀⢸⡇⢸⣍⣉⣉⣉⣉⣉⣉⣉⣉⣹⡇⢸⣏⣉⣉⣉⣉⣉⣉⣉⣉⣩⡇⢸⡇⠀
                                                                            ⠀⠘⠃⢸⣭⣭⣭⣭⣭⣭⣭⣭⣭⣽⡇⢸⣯⣭⣭⣭⣭⣭⣭⣭⣭⣭⡇⠘⠃⠀
                                                                            ⠀⠀⢸⣿⣥⣤⣤⣤⣤⣤⣤⣤⣤⣼⡇⢸⣧⣤⣤⣤⣤⣤⣤⣤⣤⣬⣿⡇⠀⠀
                                                                            ⠀⠀⢸⣿⣤⣤⣤⣤⣤⣤⣤⣤⣤⣼⡇⢸⣧⣤⣤⣤⣤⣤⣤⣤⣤⣤⣿⡇⠀⠀
                                                                            ⠀⠀⢸⣷⣤⣤⣤⣤⣤⣤⣤⣤⣤⣼⡇⢸⣧⣤⣤⣤⣤⣤⣤⣤⣤⣤⣾⡇⠀⠀
                                                                            ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
                    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                   \s
                                        """;
            System.out.println(greeting);


            //démarrage IHM
            while (running){

            socket.receive(message);//programme bloque jusqu'à ce que le processus nous envoie un message
            InetAddress clientAddress = message.getAddress();//recupération cordonnées du processus controle
            int clientPort = message.getPort();
            int typeBiere = Integer.parseInt(new String(message.getData(), 0, message.getLength()));
                try {//relier le stub (réference locale) de l'interface à l'objet implémenté chez fournisseur
                    IBiere stub = (IBiere)Naming.lookup("rmi://localhost:2020/Fournisseur");
                    switch(typeBiere){
                        case 1:
                            System.out.println("Voici la liste de nos bières Ambrées:");
                            for (Biere bir : stub.listeAmbrees()) {
                                System.out.println(bir.getNom());
                            }
                            break;
                        case 2:
                            System.out.println("Voici la liste de nos bières Blondes");
                            for (Biere bir : stub.listeBlondes()) {
                                System.out.println(bir.getNom());
                            }
                            break;
                        case 3://message envoyé lorsque le processus reçoit un signal d'interruption
                            System.out.println("Connexion Interrompue");
                            running = false;
                        default:
                            System.out.println("Requete Invalide");
                    }
                    String nextRequete = scanner.nextLine(); //lecture du nom de la bière qu'on souhaite commander
                    Biere bir2 = stub.acheterBiere(nextRequete);//appel méthode remote

                    while (bir2 == null){//cas où la bière n'as pas été trouvé, on boucle jusqu'à ce que l'utilisateur indinque une disponible
                        System.out.println("La bière en question n'est pas disponible, veuillez choisir une autre");
                        nextRequete = scanner.nextLine();
                        bir2 = stub.acheterBiere(nextRequete); 
                    }
                    buffer = nextRequete.getBytes();//Renvoyer le nom de la bière achetée vers le processus controle
                    DatagramPacket response = new DatagramPacket(buffer, buffer.length, clientAddress, clientPort);
                    socket.send(response);
                }
                catch (Exception ae){
                    System.out.println(ae);
                    running = false;
                }
            }
            socket.close();
        }
        catch (SocketException se){
            System.out.println(se);
        }



    }
}
