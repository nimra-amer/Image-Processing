#include <iostream>
#include <fstream>
#include <vector>
#include<cstring>
#include<cmath>
using namespace std;

void B_Sort(int a[], int s){
    for(int i = 0; i< s-1; i++){
        for(int j= 0; j< s-2-i; j++){
            if(a[j] < a[j+1]){
                int T = a[j];
                a[j] = a[j+1];
                a[j+1] = T;
            }
        }
    }
}

struct PGMImage{
    int H, W, MaxGray;
    string Comment;
    string MagicNo;
    vector<vector<int>> ImageData;

    PGMImage(){
        cout<<"Called Automatically"<<endl;
        H = W = MaxGray = -1;
    }

    void Create(vector<vector<int>>& Img, int H, int W){
        Img = vector<vector<int>>(H);
        for(int i = 0; i< H; i++)
            Img[i] = vector<int>(W);
        return;
    }

    bool ReserveMemory(){
        // Code to create memory storage for the image
        // This code assumes that H and W have been set earlier
        cout<<W<<" "<<H<<endl;
        if(H < 0 || W < 0)
            return false;
        Create(ImageData, H, W);
        return true;
    }

    bool LoadImage(char FileName[]){
        // Code to open File, Reserve Memory and load image data in memory
        ifstream FIN(FileName);
        if(! FIN.is_open())
            return false;

        getline(FIN, MagicNo);
        getline(FIN, Comment); // reading the comment. PROBLEM HERE THE COMMENTIS OPTIONAL

        FIN >> W >> H >> MaxGray;
        if( !ReserveMemory()){
            FIN.close();
            return false;
        }

        for(int r = 0; r < H; r++){
            for(int c = 0; c < W; c++){
                FIN >> ImageData[r][c];
            }
        }

        FIN.close();
        return true;
    }

    bool SaveImage(char FileName[]){
        // Code to open File, Reserve Memory and load image data in memory
        if(H < 0)
            return false;

        ofstream FOUT(FileName);
            // Your Code to check if the file has been created for output
            // If File not created then return false

        FOUT << "P2" <<endl
             << "# Created By MB For BDS-1A and BDS-1C"<<endl
             << W <<" " <<H<<endl
             <<MaxGray<<endl;

        for(int r = 0; r < H; r++){
            for(int c = 0; c < W; c++){
                FOUT << ImageData[r][c]<<" ";
            }
            FOUT << endl;
        }
        FOUT.close();
        return true;
    }

    bool Create_Negative(){
        if(H < 0)
            return false;
        for(int r = 0; r < H; r++){
            for(int c = 0; c < W; c++)
                ImageData[r][c] = MaxGray - ImageData[r][c];
        }
        return true;
    }

    bool Mean_Filter(int FilterSize=3){
        // create a new image in memory
        // apply median filter and then copy the result
        // back to the same image
        if(H < 0)
            return false;
        vector<vector<int>> Temp;
        Create(Temp, H, W);
        for(int r = 0; r < H; r++){
            for(int c = 0; c < W ; c++){
                int k = 0;
                int SUM = 0;
                for(int i=r-FilterSize/2; i< r+FilterSize/2; i++){
                    for(int j= c-FilterSize/2; j< c+FilterSize/2; j++){
                        if(i >=0 && i < H && j >= 0 && j < W){
                            SUM+= ImageData[i][j];
                            k++;
                        }
                    }
                }
                Temp[r][c] = SUM/k;
            }
        }
        for(int r = 0; r < H; r++){
            for(int c = 0; c < W ; c++){
                ImageData[r][c] = Temp[r][c];
            }
        }
    }

    bool Median_Filter(int FilterSize=3){
        // create a new image in memory
        // apply median filter and then copy the result
        // back to the same image
        if(H < 0)
            return false;
        vector<vector<int>> Temp;
        Create(Temp, H, W);
        int D[100];
        for(int r = 0; r < H; r++){
            for(int c = 0; c < W ; c++){
                int k = 0;
                for(int i=r-FilterSize/2; i< r+FilterSize/2; i++){
                    for(int j= c-FilterSize/2; j< c+FilterSize/2; j++){
                        if(i >=0 && i < H && j >= 0 && j < W){
                            D[k] = ImageData[i][j];
                            k++;
                        }
                    }
                }
                B_Sort(D, k);
                Temp[r][c] = D[k/2];
            }
        }
        for(int r = 0; r < H; r++){
            for(int c = 0; c < W ; c++){
                ImageData[r][c] = Temp[r][c];
            }
        }
        return true;
    }

    bool Rotate(double angle){
        vector<vector<int>> Temp;
        Create(Temp , H , W);
        for(int i=0 ; i < H ; i++){
            for(int j=0 ; j < W ; j++){
                int x = i*cos(angle) - j*sin(angle);
                int y = i*sin(angle) + j*cos(angle);
                if(x > 0 && x < H && y > 0 && y < W)
                    Temp[x][y] = ImageData[i][j];
            }
        }
        for(int i = 0; i< H ; i++){
            for(int j = 0 ; j< W ; j++){
                ImageData[i][j] = Temp[i][j];
            }
        }

        return true;
    }

    bool RotateClockwise(){
        /*if(H < 0 || W < 0){
                return false;
        }*/
        for(int i=0 ; i < H/2 ; i++)
            {
            for(int j=0; j < H-i-1; ++j)
              {
            int temp = ImageData[i][j];
            ImageData[i][j]=ImageData[H-1-j][i];
            ImageData[H-1-j][i] = ImageData[H-i-j][H-i-j];
            ImageData[H-i-j][H-i-j] = ImageData[j][H-i-1];
            ImageData[j][H-i-1] = temp;
              }
            }

        return true;
    }

    bool RotateCounterClockwise(){

        if(H<0 || W<0)
        {
            return false;
        }

        for( int i=0 ; i<W ; i++){
            for(int j=0 ; j<H ; j++){

               int temp = ImageData[(H-1)-j][i];
               ImageData [(H-1)-j][i] = ImageData[i][j];
               ImageData[i][j] = temp;

            }
        }
           /* for(int i=0 ; i<W/2 ; i++){
                for(int j=0 ; j< W-i-1 ; j++){
                     int temp=ImageData[i][j];
          ImageData[i][j] = ImageData[j][W-i-1];
          ImageData[j][W-i-1] = ImageData[W-i-1][W-j-1];
          ImageData[W-i-1][W-j-1] = ImageData[W-j-1][i];
          ImageData[W-j-1][i] = temp;*/
        return true;
    }



    bool FlipHorizontal(){
     if(H < 0 || W < 0)
     {
        return false;
     }
     for(int y = 0; y < H ; y++){
        for(int i=0 , j = W-1 ; i<j ; i++ , j--){
            int tempPixel = ImageData[y][i];
            ImageData[y][i] = ImageData[y][j];
            ImageData[y][j] = tempPixel;
        }
     }
        return true;
    }

    bool FlipVertical(){
        if(H < 0 || W <0)
        {
            return false;
        }
        for(int x=0; x < W ; x++){
          for(int i=0,j= H-1; i<j ; i++ , j--){
            int tempPixel = ImageData[i][x];
            ImageData[i][x] = ImageData[j][x];
            ImageData[j][x] = tempPixel;
          }
        }

        return true;
    }

    bool Resize(int NewH, int NewW){
      bool valid = false;
//    int NewH = 0, NewW = 0;
    do {
        cout<<"Enter New Width and Height\n";
        cin >> NewW >> NewH;
        if (NewW > 0 && NewH > 0) {
            valid = true;
        } else {
            cout << "Wrong Input, Please try again\n";
        }
    } while (!valid);

    vector<vector<int> > TempImage (NewH, vector<int> (NewW, 0));

    double scaleWidth =  (double)NewW / (double)W;
    double scaleHeight = (double)NewH / (double)H;
    double threshold = 0.5 / (scaleWidth * scaleHeight);
    double yend = 0.0;

    for(int cy = 0; cy < NewH; cy++)
    {
        double ystart = yend;
        yend = (cy + 1) / scaleHeight;
        if (yend >= H) {
            yend = H - 0.000001;
        }
        double xend = 0.0;

        for(int cx = 0; cx < NewW; cx++)
        {
            double xstart = xend;
            xend = (cx + 1) / scaleWidth;
            if (xend >= W) {
                xend = W - 0.000001;
            }
            double sum = 0.0;

            for (int y = (int)ystart; y <= (int)yend; y++) {
                double yportion = 1.0;
                if (y == (int)ystart) yportion -= ystart - y;
                if (y == (int)yend) yportion -= y+1 - yend;
                for (int x = (int)xstart; x <= (int)xend; ++x)
                {
                    double xportion = 1.0;
                    if (x == (int)xstart) xportion -= xstart - x;
                    if (x == (int)xend) xportion -= x+1 - xend;
                    sum += ImageData[y][x] * yportion * xportion;
                }
            }

            TempImage[cy][cx] = (sum > threshold) ? 1 : 0;
        }
    }

    ImageData = TempImage;
        return true;
    }

    bool CropImage(int Left, int Top, int Right, int Bottom){
        bool valid = false;
    int TopLeftCornerX = 0, TopLeftCornerY = 0, newheight = 0, newwidth = 0;
    do {
        cout<<"Enter image Top Left Index (x,y)";
        cin >> TopLeftCornerX >> TopLeftCornerY;
        cout << "Enter the cropped image width and height\n";
        cin >> newwidth >> newheight;
        if (newwidth > 0 && newheight > 0 && newwidth < W && TopLeftCornerX < W && newheight < H && TopLeftCornerY < H) {
            valid = true;
        } else {
            cout << "Wrong Input, Please try again\n";
        }
    } while (!valid);

    vector<vector<int> > TempImage (newheight, vector<int> (newwidth, 0));

    for(int i = 0; i < newheight; i++) {
        for (int j = 0; j < newwidth; j++) {
            TempImage[i][j] = ImageData[TopLeftCornerY+i][TopLeftCornerX+j];
        }
    }

    ImageData = TempImage;
    H = newheight;
    W = newwidth;

        return true;
    }

    bool ApplyTransformation(double TrasformationMatrix[][3]){

        return true;
    }
};

int Menu(){
    int choice;
    do{
        cout<<endl<<"1. Load Image"<<endl
        <<"2. Save Image"<<endl
        <<"__________________________"<<endl
        <<"3. Create Negative Image"<<endl
        <<"__________________________"<<endl
        <<"4. Mean Filter"<<endl
        <<"5. Median Filter"<<endl
        <<"__________________________"<<endl
        <<"6. Flip Horizontal"<<endl
        <<"7. Flip Vertical"<<endl
        <<"8. Rotate Clockwise by 90 Degree"<<endl
        <<"9. Rotate Counter Clockwise by 90 degree"<<endl
        <<"10. Rotate By arbitrary angle"<<endl
        <<"11. Resize Image"<<endl
        <<"12. Apply general Linear Transformation"<<endl
        <<"13. Crop Image"<<endl
        <<"__________________________"<<endl
        <<"14. Exit"<<endl<<endl
        <<"Enter Your Choice: ";
        cin>> choice;
    } while(choice < 1 || choice > 14);
}

int main()
{
    PGMImage Image;

    char FileName[100];
    int Choice;

    do{
        Choice = Menu();
        if(Choice == 1){
            cout<<"Enter File Name ";
            cin>>FileName;
            if(Image.LoadImage(FileName) )
               cout<<"Image Loaded Successfully "<<endl;
            else
                cout<<"Image Not Loaded Successfully "<<endl;
        }
        else if(Choice == 2){
            cout<<"Enter File Name ";
            cin >> FileName;
            if(Image.SaveImage(FileName) )
               cout<<"Image Saved Successfully "<<endl;
            else
                cout<<"Image Not Saved Successfully "<<endl;

        }
        else if(Choice == 3){
            if(Image.H < 0)
                cout<<"No Image Loaded"<<endl;
            else
                Image.Create_Negative();
        }
        else if(Choice == 4){
            Image.Mean_Filter(5);
        }
        else if(Choice == 5){
            int S;
            cout<<"Filter Size ";
            cin>> S;
            Image.Median_Filter(S);
        }
        else if(Choice == 6)
        {
            Image.FlipHorizontal();
            cout<<"The image is successfully horizontally flipped";
;        }
        else if(Choice == 7){
                 Image.FlipVertical();
                 cout<<"The image is successfully vertically flipped";

        }
        else if(Choice == 8){
             Image.RotateClockwise();
             if(Image.RotateClockwise()== true)
             {
             cout<<"the image is successfully rotated by 90 degrees";
             }
             else
                cout<<"action not performed successfully";
             }
        else if(Choice == 9){
               Image.RotateCounterClockwise();
               if(Image.RotateCounterClockwise() == true){
               cout<<"the image is sucessfully rotated clockwise by 90 degrees";
               }
               else
                cout<<"action not performed successfully";
        }
        else if(Choice == 10){
                double angle;
                double pi = 3.14;

             cout<<"Enter the angle to rotate the image";
             cin>>angle;
             angle=pi*angle/180;
             Image.Rotate(angle);
        }
        else if(Choice == 11){
            int NewH , NewW;
            Image.Resize( NewH, NewW);
            cout<<"The image is successfully resized according to the new input";

        }
        else if(Choice == 12){

        }
        else if(Choice == 13){
                int Left = 0, Top = 0 , Right = 0 , Bottom = 0;
         Image.CropImage( Left,  Top,  Right,  Bottom);
         cout<<"the image is successfully cropped according to the user requirements";

        }

    }while(Choice != 14);

    return 0;

}
