class Transaction < ActiveRecord::Base
  has_many :modifications
  belongs_to :renter
  #belongs_to :host   #changed by megha
  belongs_to :room    #changed by megha
  puts 'GETS TO TRANSACTION \n'
  
end
