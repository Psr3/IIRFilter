function [y,RegX]= filtreForm1(Signal,RegX,NumCoeff, DenCoeff,Gain)
[m,n]=size(Signal);
y=zeros(1,n);
for k=1:1:n
    RegX(1,3) = RegX(1,2);
    RegX(1,2) = RegX(1,1);
    RegX(1,1) = Signal(1,k);
    
    %1er etage
    RegX(2,3) = RegX(2,2);
    RegX(2,2) = RegX(2,1);
    RegX(2,1) = NumCoeff(1)*RegX(1,1)+NumCoeff(2)*RegX(1,2)+NumCoeff(3)*RegX(1,3)-DenCoeff(1,2)*RegX(2,2)-DenCoeff(1,3)*RegX(2,3);
    
    %2e etage
    RegX(3,3) = RegX(3,2);
    RegX(3,2) = RegX(3,1);
    RegX(3,1) = (NumCoeff(1)*RegX(2,1)+NumCoeff(2)*RegX(2,2)+NumCoeff(3)*RegX(2,3))*Gain(1)-DenCoeff(2,2)*RegX(3,2)-DenCoeff(2,3)*RegX(3,3);
    
    
    %3e etage
    RegX(4,3) = RegX(4,2);
    RegX(4,2) = RegX(4,1);
    RegX(4,1) = (NumCoeff(1)*RegX(3,1)+NumCoeff(2)*RegX(3,2)+NumCoeff(3)*RegX(3,3))*Gain(2)-DenCoeff(3,2)*RegX(4,2)-DenCoeff(3,3)*RegX(4,3);
    
    
    %4e etage
    RegX(5,3) = RegX(5,2);
    RegX(5,2) = RegX(5,1);
    RegX(5,1) = (NumCoeff(1)*RegX(4,1)+NumCoeff(2)*RegX(4,2)+NumCoeff(3)*RegX(4,3))*Gain(3)-DenCoeff(4,2)*RegX(5,2)-DenCoeff(4,3)*RegX(5,3);
 
    y(1,k)=RegX(5,1)*Gain(4);

end
end